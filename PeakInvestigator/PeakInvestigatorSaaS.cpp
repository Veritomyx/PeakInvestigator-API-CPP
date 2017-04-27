// --------------------------------------------------------------------------
//  PeakInvestigator-API -- C++ library for accessing the public API of
//                              PeakInvestigator.
// --------------------------------------------------------------------------
// Copyright Veritomyx, Inc. 2016.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Adam Tenderholt $
// $Author: Adam Tenderholt $
// --------------------------------------------------------------------------
//
//

#include <stdexcept>
#include <sstream>
#include <limits>

#include <curl/curl.h>
#include <libssh2.h>
#include <libssh2_sftp.h>

#include "PeakInvestigatorSaaS_config.h"

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include "PeakInvestigatorSaaS.h"
#include "AbstractProgress.h"
#include "Actions/BaseAction.h"
#include "Actions/SftpAction.h"

#include <spdlog/spdlog.h>

// states of connection for SFTP
#define LIBRARY_UNINITIALIZED     0
#define LIBRARY_INITIALIZED       1
#define SOCKET_CONNECTED          3
#define SSH_SESSION_INITIALIZED   7
#define SSH_SESSION_ESTABLISHED   15
#define SFTP_SESSION_ESTABLISHED  31

// state modification defines
#define INITIALIZE_LIBRARY        1
#define CONNECT_SOCKET            2
#define INITIALIZE_SSH_SESSION    4
#define ESTABLISH_SSH_SESSION     8
#define ESTABLISH_SFTP_SESSION    16

#define BUFFER_SIZE 131072
#define KEY_SIZE 16
#define TIMEOUT 10000
#define LOG_NAME "peakinvestigator"

using namespace Veritomyx::PeakInvestigator;

size_t process_response(void* contents, size_t size, size_t nmemb, void* target)
{
  size_t realsize = size * nmemb;
  std::string* response = reinterpret_cast<std::string*>(target);
  response->append(reinterpret_cast<char*>(contents), realsize);
  return realsize;
}

PeakInvestigatorSaaS::PeakInvestigatorSaaS(std::string hostname, std::string path, std::string agent)
{
  hostname_ = hostname;
  path_ = path;
  agent_ = agent;

  auto log = spdlog::stdout_color_mt(LOG_NAME);

  curl_global_init(CURL_GLOBAL_ALL);

  socket_ = 0;
  ssh_session_ = NULL;
  sftp_session_ = NULL;

  state_ = LIBRARY_UNINITIALIZED;

#ifdef _WIN32
  WSADATA wsadata;
  int err;

  err = WSAStartup(MAKEWORD(2, 0), &wsadata);
  if (err != 0)
  {
	spdlog::get("peakinvestigator")->error("Unable to start winsock.\n");
	throw std::runtime_error("Unable to start winsock.");
  }
#endif

  if (libssh2_init(0) != 0)
  {
    throw std::runtime_error("Unable to initialize SSH library.");
  }

  state_ |= INITIALIZE_LIBRARY;

}

PeakInvestigatorSaaS::~PeakInvestigatorSaaS()
{
  disconnect_();
  curl_global_cleanup();
  libssh2_exit();
  spdlog::drop(LOG_NAME);
}

void PeakInvestigatorSaaS::setDebug(bool debug)
{
  if (debug)
  {
    spdlog::get(LOG_NAME)->set_level(spdlog::level::debug);
  }
  else
  {
    spdlog::get(LOG_NAME)->set_level(spdlog::level::info);
  }
}

std::string PeakInvestigatorSaaS::executeAction(BaseAction *action)
{
  CURL* curl;
  CURLcode retval;

  curl = curl_easy_init();
  if(!curl)
  {
    throw std::runtime_error("Unable to initialize CURL easy.");
  }

  auto log = spdlog::get(LOG_NAME);

  std::string url = "https://" + hostname_ + path_;
  std::string response;
  response.reserve(1000);

  std::string query = action->buildQuery();
  log->debug("Query: {}", query);

  // need to copy query to a C-string for some reason
  char* postData = new char[query.size() + 1];
  std::strcpy(postData, query.c_str());

  curl_easy_setopt(curl, CURLOPT_URL,  url.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  postData);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_response);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&response));
  curl_easy_setopt(curl, CURLOPT_USERAGENT, agent_.c_str());

  retval = curl_easy_perform(curl);

  delete[] postData;
  curl_easy_cleanup(curl);

  if(retval != CURLE_OK)
  {
    throw std::runtime_error("Problem with CURL");
  }

  log->debug("Reponse: {}", response);
  return response;
}

void PeakInvestigatorSaaS::uploadFile(SftpAction& action, std::string localFilename, std::string remoteFilename, AbstractProgress* progress)
{
  std::ifstream file(localFilename);
  if(!file)
  {
    throw std::runtime_error("Unable to open local file: " + localFilename);
  }

  establishSSHSession_(action);
  authenticateUser_(action);
  establishSFTPSession_();

  LIBSSH2_SFTP_HANDLE* sftp = getSftpHandle(sftp_session_, UPLOAD, remoteFilename.c_str());
  if(!sftp)
  {
    file.close();
    throw std::runtime_error("Unable to open remote file: " + remoteFilename);
  }

  if(progress)
  {
    const std::streampos start = file.tellg();
    auto max_size = (std::numeric_limits<std::streamsize>::max)();
    file.ignore((std::numeric_limits<std::streamsize>::max)());
    progress->initialize(file.gcount(), "Uploading file to SFTP server...");
    file.clear();
    file.seekg(start);
  }

  uploadFile_(file, sftp, progress);

  if(progress)
  {
    progress->finish();
  }

  file.close();
  libssh2_sftp_close_handle(sftp);

  disconnect_();
}

void PeakInvestigatorSaaS::downloadFile(SftpAction& action, std::string remoteFilename, std::string localFilename, AbstractProgress* progress)
{
  std::ofstream file(localFilename);
  if(!file)
  {
    throw std::runtime_error("Unable to open local file: " + localFilename);
  }

  establishSSHSession_(action);
  authenticateUser_(action);
  establishSFTPSession_();

  LIBSSH2_SFTP_HANDLE* sftp = getSftpHandle(sftp_session_, DOWNLOAD, remoteFilename.c_str());
  if(!sftp)
  {
    file.close();
    throw std::runtime_error("Unable to open remote file: " + remoteFilename);
  }

  if(progress)
  {
    LIBSSH2_SFTP_ATTRIBUTES attributes;
    int retval = libssh2_sftp_fstat(sftp, &attributes);
    if(retval != 0)
    {
      file.close();
      libssh2_sftp_close_handle(sftp);
      throw std::runtime_error("Problem getting stats about remote file: " + remoteFilename);
    }
    progress->initialize(attributes.filesize, "Downloading file from SFTP server...");
  }

  downloadFile_(file, sftp, progress);

  if(progress)
  {
    progress->finish();
  }

  file.close();
  libssh2_sftp_close_handle(sftp);

  disconnect_();
}

void PeakInvestigatorSaaS::establishSSHSession_(SftpAction& action)
{
  spdlog::get(LOG_NAME)->debug("Trying to establish SSH session.");

  ssh_session_ = libssh2_session_init();
  if(!ssh_session_)
  {
    throw std::runtime_error("Unable to initalize SSH session.");
  }

  state_ |= INITIALIZE_SSH_SESSION;

  socket_ = getConnectedSocket(action.getHost().c_str(),
                                std::to_string(action.getPort()).c_str());

  libssh2_session_set_timeout(ssh_session_, TIMEOUT);
  libssh2_session_set_blocking(ssh_session_, 1);
  int retval = libssh2_session_handshake(ssh_session_, socket_);
  if (retval != 0)
  {
    std::string error = "SSH session handshake failed: " + std::to_string(retval);
    throw std::runtime_error(error);
  }

  confirmSSHServerIdentity_(action);

  state_ |= ESTABLISH_SSH_SESSION;
}

void PeakInvestigatorSaaS::confirmSSHServerIdentity_(SftpAction& action)
{
  char hex[3];
  const char* hash = libssh2_hostkey_hash(ssh_session_, LIBSSH2_HOSTKEY_HASH_MD5);
  std::string hashString(KEY_SIZE * 3 - 1, ':');
  for(int i = 0; i < KEY_SIZE; i++) {
    snprintf(hex, 3, "%02x", static_cast<unsigned char>(hash[i]));
    hashString.replace(3 * i, 2, hex);
  }

  std::string fingerprint = action.getFingerprints().getHash("RSA-MD5");
  if (fingerprint.compare(hashString) != 0) {
    std::stringstream stream;
    stream << "Unable to confirm SSH server identity. Expected: " << fingerprint;
    stream << " , found: " << hashString;
    throw std::runtime_error(stream.str());
  }
}

void PeakInvestigatorSaaS::authenticateUser_(SftpAction& action)
{
  int retval = libssh2_userauth_password(ssh_session_, action.getSftpUsername().c_str(),
                                         action.getSftpPassword().c_str());
  if (retval != 0)
  {
    throw std::runtime_error("SSH username and/or password incorrect.");
  }
}

void PeakInvestigatorSaaS::establishSFTPSession_()
{
  sftp_session_ = libssh2_sftp_init(ssh_session_);
  if (!sftp_session_)
  {
    throw std::runtime_error("Unable to start SFTP session.");
  }

  state_ |= ESTABLISH_SFTP_SESSION;
}

void PeakInvestigatorSaaS::disconnect_()
{
  spdlog::get(LOG_NAME)->debug("Disconnecting. The current state is: {}.", state_);

  switch(state_)
  {
  case SFTP_SESSION_ESTABLISHED:
    libssh2_sftp_shutdown(sftp_session_);
    state_ ^= ESTABLISH_SFTP_SESSION;
  case SSH_SESSION_ESTABLISHED:
    libssh2_session_disconnect(ssh_session_, "Disconnecting.");
    state_ ^= ESTABLISH_SSH_SESSION;
  case SSH_SESSION_INITIALIZED:
    libssh2_session_free(ssh_session_);
    state_ ^= INITIALIZE_SSH_SESSION;
  case SOCKET_CONNECTED:
#ifdef _WIN32
    closesocket(socket_);
#else
    close(socket_);
#endif
    state_ ^= CONNECT_SOCKET;
  case LIBRARY_INITIALIZED:
    break;
  default:
    spdlog::get(LOG_NAME)->error("Problem in disconnect. State: {}.", state_);
  }

}

#pragma comment(lib, "Ws2_32.lib")
int PeakInvestigatorSaaS::getConnectedSocket(const char* host, const char* port)
{
  // setup hints to for host lookup
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;

  int retval = getaddrinfo(host, port, &hints, &host_info_);
  if (retval != 0)
  {
    std::string error = "Unable to resolve host ";
    error.append(host);
    error.append(" : ");
    error.append(std::to_string(retval));
    throw std::runtime_error(error);
  }

  int sock = socket(host_info_->ai_family, host_info_->ai_socktype, host_info_->ai_protocol);
  retval = connect(sock, host_info_->ai_addr, host_info_->ai_addrlen);
  freeaddrinfo(host_info_);

  if (retval != 0)
  {
    std::string error = "Unable to connect socket.";
    throw std::runtime_error(error);
  }

  state_ |= CONNECT_SOCKET;
  return sock;
}

_LIBSSH2_SFTP_HANDLE* PeakInvestigatorSaaS::getSftpHandle(_LIBSSH2_SFTP* sftp_session, Direction direction, const char* filename)
{
  int flags, mode;
  switch(direction)
  {
  case UPLOAD:
    flags = LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC;
    mode = LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IRGRP | LIBSSH2_SFTP_S_IROTH;
    return libssh2_sftp_open(sftp_session, filename, flags, mode);
  case DOWNLOAD:
    flags = LIBSSH2_FXF_READ;
    mode = 0;
    return libssh2_sftp_open(sftp_session, filename, flags, mode);
  default:
    throw std::runtime_error("SFTP direction not correctly specified.");
  }

  return NULL;
}

void PeakInvestigatorSaaS::uploadFile_(std::ifstream& file, _LIBSSH2_SFTP_HANDLE* sftp, AbstractProgress* progress)
{
  int written, transferred = 0;
  char buffer[BUFFER_SIZE];

  auto log = spdlog::get(LOG_NAME);
  log->debug("Uploading file...");

  while(!file.eof())
  {
    file.read(buffer, BUFFER_SIZE);
    int read = file.gcount();
    log->debug("... read {} of {} possible bytes.", read, BUFFER_SIZE);

    char* ptr = buffer;
    while(read)
    {
      written = libssh2_sftp_write(sftp, ptr, read);
      if (written < 0)
      {
        file.close();
        libssh2_sftp_close_handle(sftp);

        std::string message = "Problem writing bytes to server: " + std::to_string(read);
        throw std::runtime_error(message);
      }

      ptr += written;
      transferred += written;
      read -= written;

      if(progress)
      {
        progress->setProgress(transferred);
      }
    }

  }

  spdlog::get(LOG_NAME)->debug("....finished.");

}

void PeakInvestigatorSaaS::downloadFile_(std::ofstream& file, _LIBSSH2_SFTP_HANDLE* sftp, AbstractProgress* progress)
{
  int read, transferred = 0;
  char buffer[BUFFER_SIZE];

  spdlog::get(LOG_NAME)->debug("Downloading file...");

  do
  {
    int read = libssh2_sftp_read(sftp, buffer, BUFFER_SIZE);
    if (read == 0)
    {
      break; // EOF
    }
    else if (read < 0)
    {
      std::string message = "Error while reading bytes from server: " + std::to_string(read);
      throw std::runtime_error(message);
    }

    file.write(buffer, read);
    if(!file.good())
    {
      std::string message = "Error while writing local file: " + std::to_string(read);
      throw std::runtime_error(message);
    }

    transferred += read;
    if(progress)
    {
      progress->setProgress(transferred);
    }

  } while(true);

  spdlog::get(LOG_NAME)->debug("Finished.");
}
