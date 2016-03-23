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
