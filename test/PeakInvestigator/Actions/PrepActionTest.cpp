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

#include <PeakInvestigator/Actions/PrepAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(PrepActionTest, QueryString)
{
  PrepAction action("username", "password", 1234, "example.tar");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=PREP&ID=1234&File=example.tar",
               action.buildQuery().c_str());
}

// "{\"Action\":\"PREP\", \"File\":\"WatersQ-TOF.tar\", \"Status\":\"Analyzing\", \"PercentComplete\":\"90%\"}"
TEST(PrepActionTest, ExampleResponse_Analyzing)
{
  PrepAction action("username", "password", 1234, "example.tar");
  action.processResponse(PrepAction::EXAMPLE_RESPONSE_ANALYZING);

  ASSERT_STREQ("WatersQ-TOF.tar", action.getFilename().c_str());
  ASSERT_EQ(PrepAction::ANALYZING, action.getStatus());
  ASSERT_STREQ("90%", action.getPercentComplete().c_str());
}

// "{\"Action\":\"PREP\", \"File\":\"AV001.tar\", \"Status\":\"Ready\", \"PercentComplete\":\"100%\", \"MSType\":\"TOF\", \"ScanCount\":1619, \"MaxPoints\":384905, \"MinMass\":59, \"MaxMass\":1699}");
TEST(PrepActionTest, ExampleResponse_Ready)
{
  PrepAction action("username", "password", 1234, "example.tar");
  action.processResponse(PrepAction::EXAMPLE_RESPONSE_READY);

  ASSERT_STREQ("AV001.tar", action.getFilename().c_str());
  ASSERT_EQ(PrepAction::READY, action.getStatus());
  ASSERT_STREQ("100%", action.getPercentComplete().c_str());
  ASSERT_STREQ("TOF", action.getMStype().c_str());
  ASSERT_EQ(1619, action.getScanCount());
  ASSERT_EQ(384905, action.getMaxPoints());
  ASSERT_EQ(59, action.getMinMass());
  ASSERT_EQ(1699, action.getMaxMass());
}
