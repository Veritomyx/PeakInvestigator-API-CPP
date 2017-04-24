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

#include "SftpAction.h"

using namespace Veritomyx::PeakInvestigator;

const std::string SftpAction::EXAMPLE_RESPONSE("{\"Action\":\"SFTP\", \"Host\":\"peakinvestigator.veritomyx.com\", \"Port\":22022, \"Directory\":\"/files\", \"Login\":\"Vt504\", \"Password\":\"0UtnWMvzoi2jF4BQ\", \"Fingerprints\":[ {\"Signature\":\"DSA\",\"Algorithm\":\"MD5\",\"Hash\":\"96:bd:da:62:5a:53:1a:2f:82:87:65:7f:c0:45:71:94\"}, {\"Signature\":\"DSA\",\"Algorithm\":\"SHA256\",\"Hash\":\"b9SOs40umHMywBa2GtdsOhr/wgP1L6nfXWugjRrJTaM\"}, {\"Signature\":\"ECDSA\",\"Algorithm\":\"MD5\",\"Hash\":\"5c:6f:c7:c7:79:c0:76:90:4d:3a:a1:7a:81:0e:0a:57\"}, {\"Signature\":\"ECDSA\",\"Algorithm\":\"SHA256\",\"Hash\":\"d2HXgeUSmWN+gq+9V7Wad5xWaCxk+mh45F81K951MCU\"}, {\"Signature\":\"RSA\",\"Algorithm\":\"MD5\",\"Hash\":\"d2:be:b8:2e:3c:be:84:e4:a3:0a:c8:42:5c:6b:39:4e\"}, {\"Signature\":\"RSA\",\"Algorithm\":\"SHA256\",\"Hash\":\"QBsg8ejj4gZun4AWd4WBTJw89ftcLR9x/dZoG223srg\"}]}");

SftpAction::SftpAction(std::string user, std::string code, int project_ID) :
  BaseAction(user, code, "SFTP")
{
  project_ID_ = project_ID;
}

std::string SftpAction::buildQuery() const
{
  std::string query = BaseAction::buildQuery();
  query.append("&ID="); query.append(std::to_string(project_ID_));

  return query;
}

void SftpAction::preCheck()
{
  if (!isReady("SFTP"))
  {
    throw std::runtime_error("Response has not been set.");
  }
}

std::string SftpAction::getHost()
{
  preCheck();
  return getStringAttribute("Host");
}

int SftpAction::getPort()
{
  preCheck();
  return static_cast<int>(getLongAttribute("Port"));
}

std::string SftpAction::getDirectory()
{
  preCheck();
  return getStringAttribute("Directory");
}

std::string SftpAction::getSftpUsername()
{
  preCheck();
  return getStringAttribute("Login");
}

std::string SftpAction::getSftpPassword()
{
  preCheck();
  return getStringAttribute("Password");
}

SftpFingerprints SftpAction::getFingerprints()
{
  if (fingerprints_.size() > 0) {
    return fingerprints_;
  }

  preCheck();
  Json::Value fingerprints = getAttribute("Fingerprints");
  for (unsigned int i = 0; i < fingerprints.size(); i++)
  {
    Json::Value fingerprint = fingerprints[i];
    std::string signature = fingerprint.get("Signature", Json::Value()).asString();
    std::string algorithm = fingerprint.get("Algorithm", Json::Value()).asString();
    std::string hash = fingerprint.get("Hash", Json::Value()).asString();

    std::string key = signature + "-" + algorithm;
    fingerprints_[key] = hash;
  }

  return fingerprints_;
}

std::string SftpAction::getErrorMessage()
{
  preCheck();
  return BaseAction::getErrorMessage();
}

int SftpAction::getErrorCode()
{
  preCheck();
  return BaseAction::getErrorCode();
}

std::vector<std::string> SftpFingerprints::getAlgorithms()
{
  std::vector<std::string> algorithms;

  std::map<std::string, std::string>::const_iterator iter;
  for (iter = begin(); iter != end(); ++iter)
  {
    algorithms.push_back(iter->first);
  }

  return algorithms;
}

std::string SftpFingerprints::getHash(std::string algorithm)
{
  return (*this)[algorithm];
}
