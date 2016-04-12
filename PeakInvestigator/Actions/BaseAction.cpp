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

#include <json/json.h>

#include "BaseAction.h"

#ifdef _WIN32
char *strptime(const char *buf, const char *fmt, struct tm *tm);
#endif

using namespace Veritomyx::PeakInvestigator;

const std::string BaseAction::VERSION_OF_API("3.4");
const std::string BaseAction::PARSE_DATE_FORMAT("%Y-%m-%d %H:%M:%S");

BaseAction::BaseAction(std::string user, std::string code, std::string action)
{
  user_ = user;
  code_ = code;
  action_ = action;

  response_object_ = new Json::Value();
}

BaseAction::~BaseAction()
{
  delete response_object_;
}

std::string BaseAction::buildQuery() const
{
  std::string query;
  query.reserve(1000);
  query.append("Version="); query.append(VERSION_OF_API);
  query.append("&User=");   query.append(user_);
  query.append("&Code=");   query.append(code_);
  query.append("&Action="); query.append(action_);

  return query;
}

void BaseAction::processResponse(const std::string response)
{
  if (response.at(0) == '<')
  {
    throw std::runtime_error("Server response appears to be HTML/XML.");
  }

  Json::Reader reader;
  bool ok = reader.parse(response, *response_object_);
  if(!ok)
  {
    throw std::runtime_error("Unable to parse the response as JSON data.");
  }
}

bool BaseAction::isReady(std::string action)
{
  if (response_object_->isNull())
  {
    return false;
  }

  if (getStringAttribute("Action").compare(action) != 0)
  {
    std::string err = "Response is from another action: "
        + getStringAttribute("Action");
    throw std::invalid_argument(err);
  }

  return true;
}

bool BaseAction::hasError()
{
  if (response_object_->isNull())
  {
    throw new std::invalid_argument("Response was empty.");
  }

  return response_object_->isMember("Error");
}

std::string BaseAction::getErrorMessage()
{
  return getStringAttribute("Message");
}

int BaseAction::getErrorCode()
{
  return getIntAttribute("Error");
}

std::string BaseAction::getStringAttribute(std::string attribute) const
{
  return response_object_->get(attribute, Json::nullValue).asString();
}

int BaseAction::getIntAttribute(std::string attribute) const
{
  return response_object_->get(attribute, Json::nullValue).asInt();
}

long BaseAction::getLongAttribute(std::string attribute) const
{
  return response_object_->get(attribute, Json::nullValue).asInt64();
}

double BaseAction::getDoubleAttribute(std::string attribute) const
{
  return response_object_->get(attribute, Json::nullValue).asDouble();
}

#ifndef _WIN32
struct tm BaseAction::getDateTimeAttribute(std::string attribute) const
{
  std::string date_time_string = response_object_->get(attribute, Json::nullValue).asString();
  struct tm datetime;
  strptime(date_time_string.c_str(), PARSE_DATE_FORMAT.c_str(), &datetime);
  return datetime;
}
#endif

std::list<std::string> BaseAction::getStringListAttribute(std::string attribute) const
{
  Json::Value array = response_object_->get(attribute, Json::nullValue);
  std::list<std::string> values;
  for (unsigned int i = 0; i < array.size(); i++) {
    values.push_back(array[i].asString());
  }
  return values;
}

std::vector<std::string> BaseAction::getStringVectorAttribute(std::string attribute) const
{
  Json::Value array = response_object_->get(attribute, Json::nullValue);
  std::vector<std::string> values;
  for (unsigned int i = 0; i < array.size(); i++) {
    values.push_back(array[i].asString());
  }
  return values;
}

