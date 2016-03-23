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

#include <PeakInvestigator/Actions/PrepAction.h>

using namespace Veritomyx::PeakInvestigator;

const std::string PrepAction::EXAMPLE_RESPONSE_ANALYZING("{\"Action\":\"PREP\", \"File\":\"WatersQ-TOF.tar\", \"Status\":\"Analyzing\", \"PercentComplete\":\"90%\"}");
const std::string PrepAction::EXAMPLE_RESPONSE_READY("{\"Action\":\"PREP\", \"File\":\"AV001.tar\", \"Status\":\"Ready\", \"PercentComplete\":\"100%\", \"MSType\":\"TOF\", \"ScanCount\":1619, \"MaxPoints\":384905, \"MinMass\":59, \"MaxMass\":1699}");

PrepAction::PrepAction(std::string user, std::string code, int projectID, std::string filename) :
  BaseAction(user, code, "PREP")
{
  projectID_ = projectID;
  filename_ = filename;
}

std::string PrepAction::buildQuery() const {
  std::string query = BaseAction::buildQuery();
  query.append("&ID="); query.append(std::to_string(projectID_));
  query.append("&File="); query.append(filename_);

  return query;
}

void PrepAction::preCheck(){
  if (!isReady("PREP")) {
    throw std::runtime_error("Response has not been set.");
  }
}

std::string PrepAction::getFilename() {
  preCheck();
  return getStringAttribute("File");
}

PrepAction::Status PrepAction::getStatus() {
  preCheck();
  std::string status = getStringAttribute("Status");
  if(status == "Analyzing")
  {
    return ANALYZING;
  }
  else if(status == "Ready")
  {
    return READY;
  }
  else if(status == "Error")
  {
    return ERROR;
  }

  throw std::runtime_error("Invalid status received.");
}

std::string PrepAction::getPercentComplete() {
  preCheck();
  return getStringAttribute("PercentComplete");
}

std::string PrepAction::getMStype() {
  preCheck();
  return getStringAttribute("MSType");
}

int PrepAction::getScanCount() {
  preCheck();
  return static_cast<int>(getLongAttribute("ScanCount"));
}

int PrepAction::getMaxPoints() {
  preCheck();
  return static_cast<int>(getLongAttribute("MaxPoints"));
}

int PrepAction::getMinMass() {
  preCheck();
  return getIntAttribute("MinMass");
}

int PrepAction::getMaxMass() {
  preCheck();
  return getIntAttribute("MaxMass");
}

std::string PrepAction::getErrorMessage() {
  preCheck();
  return BaseAction::getErrorMessage();
}

int PrepAction::getErrorCode() {
  preCheck();
  return BaseAction::getErrorCode();
}
