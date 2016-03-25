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

#include "RunAction.h"

using namespace Veritomyx::PeakInvestigator;

const std::string RunAction::EXAMPLE_RESPONSE("{\"Action\":\"RUN\", \"Job\":\"P-504.1463\"}");

/** Create object for RUN API call.
         *
         * @param versionOfApi
         * @param user
         * @param code
         * @param job
         * @param RTO
         * @param inputFilename
         * @param calibrationFilename
         */
RunAction::RunAction(std::string user, std::string code, std::string job, std::string RTO,
                     std::string input_filename, std::string calibration_filename) :
  BaseAction(user, code, "RUN")
{

  job_ = job;
  RTO_ = RTO;
  input_filename_ = input_filename;
  calibration_filename_ = calibration_filename;
}

std::string RunAction::buildQuery() const
{
  std::string query = BaseAction::buildQuery();
  query.append("&Job="); query.append(job_);
  query.append("&RTO="); query.append(RTO_);
  query.append("&InputFile="); query.append(input_filename_);

  if (calibration_filename_ != "")
  {
    query.append("&CalibrationFile=");
    query.append(calibration_filename_);
  }

  return query;
}

void RunAction::preCheck()
{
  if (!isReady("RUN"))
  {
    throw std::runtime_error("Response has not been set.");
  }
}

std::string RunAction::getJob()
{
  preCheck();
  return getStringAttribute("Job");
}

std::string RunAction::getErrorMessage()
{
  preCheck();
  return BaseAction::getErrorMessage();
}

int RunAction::getErrorCode()
{
  preCheck();
  return BaseAction::getErrorCode();
}
