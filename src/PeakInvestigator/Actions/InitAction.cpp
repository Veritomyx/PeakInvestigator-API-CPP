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

#include <PeakInvestigator/Actions/InitAction.h>

using namespace Veritomyx::PeakInvestigator;

const std::string InitAction::EXAMPLE_RESPONSE("{\"Action\":\"INIT\", \"Job\":\"V-504.1551\", \"ID\":504, \"Funds\":115.01, \"EstimatedCost\":[{\"Instrument\":\"TOF\", \"RTO\":\"RTO-24\", \"Cost\":27.60}, {\"Instrument\":\"Orbitrap\", \"RTO\":\"RTO-24\", \"Cost\":36.22}, {\"Instrument\":\"IonTrap\", \"RTO\":\"RTO-24\", \"Cost\":32.59}]}");

InitAction::InitAction(std::string user, std::string code, int project_id, std::string version_of_PI, int scan_count,
                       int max_points, int min_mass, int max_mass, int calibration_count) :
  BaseAction(user, code, "INIT")
{

  project_id_ = project_id;
  version_of_PI_ = version_of_PI;
  scan_count_ = scan_count;
  max_points_ = max_points;
  min_mass_ = min_mass;
  max_mass_ = max_mass;
  calibration_count_ = calibration_count;
}

std::string InitAction::buildQuery() const
{
  std::string query = BaseAction::buildQuery();
  query.append("&ID="); query.append(std::to_string(project_id_));
  query.append("&PI_Version="); query.append(version_of_PI_);
  query.append("&ScanCount="); query.append(std::to_string(scan_count_));
  query.append("&MaxPoints="); query.append(std::to_string(max_points_));
  query.append("&MinMass="); query.append(std::to_string(min_mass_));
  query.append("&MaxMass="); query.append(std::to_string(max_mass_));
  query.append("&CalibrationCount="); query.append(std::to_string(calibration_count_));

  return query;
}

void InitAction::preCheck()
{
  if (!isReady("INIT"))
  {
    throw std::runtime_error("Failed Pre-Check");
  }
}

std::string InitAction::getJob()
{
  preCheck();
  return getStringAttribute("Job");
}

long InitAction::getProjectId()
{
  preCheck();
  return getLongAttribute("ID");
}

double InitAction::getFunds()
{
  preCheck();
  return getDoubleAttribute("Funds");
}

std::map<std::string, ResponseTimeCosts> InitAction::getEstimatedCosts()
{
  if(estimated_costs_.size() > 0)
  {
    return estimated_costs_;
  }

  preCheck();

  Json::Value estimated_costs = response_object_.get("EstimatedCost", Json::nullValue);
  for(uint i = 0; i < estimated_costs.size(); i++)
  {
    ResponseTimeCosts costs;
    std::string instrument = estimated_costs.get("Instrument", Json::nullValue).asString();
    std::string RTO = estimated_costs.get("RTO", Json::nullValue).asString();
    double cost = estimated_costs.get("Cost", Json::nullValue).asDouble();

    EstimatedCostsIterator iter = estimated_costs_.find(instrument);
    if(iter != estimated_costs_.end())
    {
      costs = iter->second;
    }
    costs[RTO] = cost;
    estimated_costs_[instrument] = costs;
  }

  return estimated_costs_;
}

std::string InitAction::getErrorMessage()
{
  preCheck();
  return BaseAction::getErrorMessage();
}

int InitAction::getErrorCode()
{
  preCheck();
  return BaseAction::getErrorCode();
}

std::list<std::string> ResponseTimeCosts::getRTOs()
{
  std::list<std::string> RTOs;

  std::map<std::string, double>::const_iterator iter;
  for (iter = begin(); iter != end(); ++iter) {
    RTOs.push_back(iter->first);
  }

  return RTOs;
}

double ResponseTimeCosts::getCost(std::string responseTimeObjective) const
{
  std::map<std::string, double>::const_iterator iter = find(responseTimeObjective);
  if(iter != end())
  {
    return iter->second;
  }

  return -1.0;
}
