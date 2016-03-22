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

#include <PeakInvestigator/Actions/DeleteAction.h>

using namespace Veritomyx::PeakInvestigator;

const std::string DeleteAction::EXAMPLE_RESPONSE("{\"Action\":\"DELETE\",\"Job\":\"P-504.4256\",\"Datetime\":\"2016-02-03 18:35:06\"}");

DeleteAction::DeleteAction(std::string user, std::string code, std::string jobID) :
  BaseAction(user, code, "DELETE")
{
  jobID_ = jobID;
}

std::string DeleteAction::buildQuery() const
{
  std::string query = BaseAction::buildQuery();
  query.append("Job="); query.append(action_);
  return query;
}

void DeleteAction::preCheck()
{
  if (!isReady(action_))
  {
    throw std::runtime_error("Response has not been set.");
  }
}

std::string DeleteAction::getJob()
{
  preCheck();
  return getStringAttribute("Job");
}

struct tm DeleteAction::getDateTime()
{
  preCheck();
  return getDateTimeAttribute("Datetime");
}

std::string DeleteAction::getErrorMessage()
{
  preCheck();
  return BaseAction::getErrorMessage();
}

int DeleteAction::getErrorCode()
{
  preCheck();
  return BaseAction::getErrorCode();
}

