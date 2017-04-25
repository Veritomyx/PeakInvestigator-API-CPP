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

#include <PeakInvestigator/Actions/PiVersionsAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(PiVersionsActionTest, QueryString)
{
  PiVersionsAction action("username", "password");
  PiVersionsAction action2(action);

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=PI_VERSIONS",
               action.buildQuery().c_str());
  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=PI_VERSIONS",
               action2.buildQuery().c_str());
}

// "{\"Action\":\"PI_VERSIONS\",\"Current\":\"1.2\",\"LastUsed\":\"\",\"Count\":2,\"Versions\":[\"1.2\",\"1.0.0\"]}"
TEST(PiVersionsActionTest, ExampleResponse)
{
  PiVersionsAction action("username", "password");
  action.processResponse(PiVersionsAction::EXAMPLE_RESPONSE_1);

  ASSERT_STREQ("1.2", action.getCurrentVersion().c_str());
  ASSERT_STREQ("", action.getLastUsedVersion().c_str());

  std::vector<std::string> versions = action.getVersions();
  ASSERT_STREQ("1.2", versions.at(0).c_str());
  ASSERT_STREQ("1.0.0", versions.at(1).c_str());

  PiVersionsAction action2(action);

  ASSERT_STREQ("1.2", action2.getCurrentVersion().c_str());
  ASSERT_STREQ("", action2.getLastUsedVersion().c_str());

  versions = action2.getVersions();
  ASSERT_STREQ("1.2", versions.at(0).c_str());
  ASSERT_STREQ("1.0.0", versions.at(1).c_str());
}
