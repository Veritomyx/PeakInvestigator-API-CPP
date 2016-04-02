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

#include "StatusAction.h"

using namespace Veritomyx::PeakInvestigator;

const std::string StatusAction::EXAMPLE_RESPONSE_PREPARING("{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Preparing\", \"Datetime\":\"2016-02-03 18:18:12\"}");
const std::string StatusAction::EXAMPLE_RESPONSE_RUNNING("{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Running\", \"Datetime\":\"2016-02-03 18:25:09\"}");
const std::string StatusAction::EXAMPLE_RESPONSE_DONE("{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Done\", \"Datetime\":\"2016-02-03 18:31:05\", \"ScansInput\":3, \"ScansComplete\":3, \"ActualCost\":0.36, \"JobLogFile\":\"\\/files\\/P-504.5148\\/P-504.5148.log.txt\", \"ResultsFile\":\"\\/files\\/P-504.5148\\/P-504.5148.mass_list.tar\"}");
const std::string StatusAction::EXAMPLE_RESPONSE_DELETED("{\"Action\":\"STATUS\", \"Job\":\"P-504.1463\", \"Status\":\"Deleted\", \"Datetime\":\"2016-02-03 18:36:05\"}");

StatusAction::StatusAction(std::string user, std::string code, std::string job) :
  BaseAction(user, code, "STATUS")
{
  job_ = job;
}

std::string StatusAction::buildQuery() const
{
  std::string query = BaseAction::buildQuery();
  query.append("&Job="); query.append(job_);

  return query;
}

void StatusAction::preCheck()
{
  if (!isReady("STATUS"))
  {
    throw std::runtime_error("Response has not been set.");
  }
}

std::string StatusAction::getJob()
{
  preCheck();
  return getStringAttribute("Job");
}

StatusAction::Status StatusAction::getStatus()
{
  preCheck();

  std::string status = getStringAttribute("Status");
  if(status == "Preparing")
  {
    return PREPARING;
  }
  else if (status == "Running")
  {
    return RUNNING;
  }
  else if(status == "Done")
  {
    return DONE;
  }
  else if(status == "Deleted")
  {
    return DELETED;
  }
  else
  {
    throw std::runtime_error("Invalid status received.");
  }
}

#ifndef _WIN32
struct tm StatusAction::getDateTime()
{
  preCheck();
  return getDateTimeAttribute("Datetime");
}
#endif

int StatusAction::getNumberOfInputScans()
{
  preCheck();
  return static_cast<int>(getLongAttribute("ScansInput"));
}

int StatusAction::getNumberOfCompleteScans()
{
  preCheck();
  return static_cast<int>(getLongAttribute("ScansComplete"));
}

double StatusAction::getActualCost()
{
  preCheck();
  return getDoubleAttribute("ActualCost");
}

std::string StatusAction::getLogFilename()
{
  preCheck();
  return getStringAttribute("JobLogFile");
}

std::string StatusAction::getResultsFilename()
{
  preCheck();
  return getStringAttribute("ResultsFile");
}

std::string StatusAction::getErrorMessage()
{
  preCheck();
  return BaseAction::getErrorMessage();
}

int StatusAction::getErrorCode()
{
  preCheck();
  return BaseAction::getErrorCode();
}
