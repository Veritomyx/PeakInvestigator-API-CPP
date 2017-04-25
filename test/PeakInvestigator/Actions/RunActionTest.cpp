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

#include <PeakInvestigator/Actions/RunAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(RunActionTest, QueryString_WithoutCalibration)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar");
  RunAction action2(action);

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar",
               action.buildQuery().c_str());
  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar",
               action2.buildQuery().c_str());
}

TEST(RunActionTest, QueryString_WithCalibration)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar", "calib.tar");
  RunAction action2(action);

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar&CalibrationFile=calib.tar",
               action.buildQuery().c_str());
  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar&CalibrationFile=calib.tar",
               action2.buildQuery().c_str());
}

// "{\"Action\":\"RUN\", \"Job\":\"P-504.1463\"}"
TEST(RunActionTest, ExampleResponse)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar");
  action.processResponse(RunAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("P-504.1463", action.getJob().c_str());

  RunAction action2(action);

  ASSERT_STREQ("P-504.1463", action2.getJob().c_str());
}
