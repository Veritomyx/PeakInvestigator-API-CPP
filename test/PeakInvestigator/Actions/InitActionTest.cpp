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

#include <PeakInvestigator/Actions/InitAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(InitActionTest, QueryString)
{
  InitAction action("username", "password", 1234, "1.2", 10, 12345, 100, 2000);

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=INIT&ID=1234&PI_Version=1.2&ScanCount=10&MaxPoints=12345&MinMass=100&MaxMass=2000&CalibrationCount=0",
                action.buildQuery().c_str());
}

// "{\"Action\":\"INIT\", \"Job\":\"V-504.1551\", \"ID\":504, \"Funds\":115.01, \"EstimatedCost\":[{\"Instrument\":\"TOF\", \"RTO\":\"RTO-24\", \"Cost\":27.60},
//                                                                                                 {\"Instrument\":\"Orbitrap\", \"RTO\":\"RTO-24\", \"Cost\":36.22},
//                                                                                                 {\"Instrument\":\"IonTrap\", \"RTO\":\"RTO-24\", \"Cost\":32.59}]}"
TEST(InitActionTest, ExampleResponse)
{
  InitAction action("username", "password", 1234, "1.2", 10, 12345, 100, 2000);
  action.processResponse(InitAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("V-504.1551", action.getJob().c_str());
  ASSERT_EQ(504, action.getProjectId());
  ASSERT_EQ(115.01, action.getFunds());

  EstimatedCosts estimated_costs = action.getEstimatedCosts();
  ASSERT_EQ(27.60, estimated_costs.forInstrument("TOF").getCost("RTO-24"));
  ASSERT_EQ(36.22, estimated_costs.forInstrument("Orbitrap").getCost("RTO-24"));
  ASSERT_EQ(32.59, estimated_costs.forInstrument("IonTrap").getCost("RTO-24"));
}
