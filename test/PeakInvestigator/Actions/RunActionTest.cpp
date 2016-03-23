#include <PeakInvestigator/Actions/RunAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(RunActionTest, QueryString_WithoutCalibration)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar",
               action.buildQuery().c_str());
}

TEST(RunActionTest, QueryString_WithCalibration)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar", "calib.tar");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=RUN&Job=P-1234&RTO=RTO-24&InputFile=example.tar&CalibrationFile=calib.tar",
               action.buildQuery().c_str());
}

// "{\"Action\":\"RUN\", \"Job\":\"P-504.1463\"}"
TEST(RunActionTest, ExampleResponse)
{
  RunAction action("username", "password", "P-1234", "RTO-24", "example.tar");
  action.processResponse(RunAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("P-504.1463", action.getJob().c_str());
}
