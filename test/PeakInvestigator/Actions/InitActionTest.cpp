#include <PeakInvestigator/Actions/InitAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(InitActionTest, QueryString)
{
  InitAction action("username", "password", 1234, "1.2", 10, 12345, 100, 2000);

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=INIT&ID=1234&PI_Version=1.2&ScanCount=10&MaxPoints=12345&MinMass=100&MaxMass=2000&CalibrationCount=0",
                action.buildQuery().c_str());
}

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
