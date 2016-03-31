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

#include <iostream>
#include <stdexcept>

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
#include "Actions/BaseAction.h"
#include "Actions/SftpAction.h"

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

#define TIMEOUT 10000
#define LOG *logger_

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

  logger_ = &std::cout;

  curl_global_init(CURL_GLOBAL_ALL);

  socket_ = 0;
  ssh_session_ = NULL;
  sftp_session_ = NULL;

  state_ = LIBRARY_UNINITIALIZED;
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

  std::string url = "https://" + hostname_ + path_;
  std::string response;
  response.reserve(1000);

  // need to copy query to a C-string for some reason
  std::string query = action->buildQuery();
  char postData[query.size() + 1];
  std::strcpy(postData, query.c_str());

  curl_easy_setopt(curl, CURLOPT_URL,  url.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS,  postData);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_response);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&response));
  curl_easy_setopt(curl, CURLOPT_USERAGENT, agent_.c_str());

  retval = curl_easy_perform(curl);

  if(retval != CURLE_OK)
  {
    throw std::runtime_error("Problem with CURL");
  }

  return response;
}

void PeakInvestigatorSaaS::uploadFile(SftpAction& action, std::string localFilename, std::string remoteFilename)
{
  establishSSHSession_(action);
  authenticateUser_(action);
  establishSFTPSession_();

  disconnect_();
}

void PeakInvestigatorSaaS::downloadFile(SftpAction& action, std::string remoteFilename, std::string localFilename)
{

}

void PeakInvestigatorSaaS::establishSSHSession_(SftpAction& action)
{
  LOG << "Trying to establish SSH session." << std::endl;

  socket_ = getConnectedSocket(action.getHost().c_str(),
                                std::to_string(action.getPort()).c_str());

  ssh_session_ = libssh2_session_init();
  if(!ssh_session_)
  {
    throw std::runtime_error("Unable to initalize SSH session.");
  }

  state_ |= INITIALIZE_SSH_SESSION;

  libssh2_session_set_timeout(ssh_session_, TIMEOUT);
  libssh2_session_set_blocking(ssh_session_, 1);
  int retval = libssh2_session_handshake(ssh_session_, socket_);
  if (retval != 0)
  {
    std::string error = "SSH session handshake failed: " + std::to_string(retval);
    throw std::runtime_error(error);
  }

  state_ |= ESTABLISH_SSH_SESSION;
}

void PeakInvestigatorSaaS::confirmSSHServerIdentity_()
{

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
  LOG << "Disconnecting. The current state is: " << state_ << std::endl;
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
    close(socket_);
    freeaddrinfo(host_info_);
    state_ ^= CONNECT_SOCKET;
  case LIBRARY_INITIALIZED:
    break;
  default:
    LOG << "Problem in disconnect. State: " << state_ << std::endl;
  }

}

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

  if (retval != 0)
  {
    std::string error = "Unable to connect socket.";
    throw std::runtime_error(error);
  }

  state_ |= CONNECT_SOCKET;
  return sock;
}
