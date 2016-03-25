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

#include "PeakInvestigatorSaaS.h"
#include "Actions/BaseAction.h"
#include "Actions/SftpAction.h"

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

  curl_global_init(CURL_GLOBAL_ALL);
}

PeakInvestigatorSaaS::~PeakInvestigatorSaaS()
{
  curl_global_cleanup();
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

void PeakInvestigatorSaaS::uploadFile(SftpAction* action, std::string localFilename, std::string remoteFilename)
{

}

void PeakInvestigatorSaaS::downloadFile(SftpAction* action, std::string remoteFilename, std::string localFilename)
{

}
