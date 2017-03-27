// --------------------------------------------------------------------------
//  PeakInvestigator-API -- C++ library for accessing the public API of
//                              PeakInvestigator.
// --------------------------------------------------------------------------
// Copyright Veritomyx, Inc. 2017.
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

#include <PeakInvestigator/Actions/BaseAction.h>
#include <PeakInvestigator/Actions/InitAction.h>
#include <PeakInvestigator/Actions/RunAction.h>
#include <PeakInvestigator/Actions/SandboxAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(SandboxInitActionTest, QueryString)
{
  JobAttributes attributes;
  attributes.max_points = 12345;
  attributes.min_mass = 100;
  attributes.max_mass = 2000;
  attributes.start_mass = 120;
  attributes.end_mass = 1900;

  InitAction init_action("username", "password", 1234, "1.2", 10, attributes);
  BaseAction* action = new SandboxAction(&init_action, 3);

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=INIT&ID=1234&PI_Version=1.2&ScanCount=10&MaxPoints=12345&MinMass=100&MaxMass=2000&StartMass=120&EndMass=1900&Sandbox=3",
               action->buildQuery().c_str());

  delete action;
}

TEST(SandboxRunActionTest, QueryStringWithCalibration)
{
  RunAction run_action("username", "password", "P-1234", "RTO-24", "example.tar");
  BaseAction* action = new SandboxAction(&run_action, "Done");

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar&Sandbox=Done",
               action->buildQuery().c_str());

  delete action;
}
