#include <PeakInvestigator/Actions/PiVersionsAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(PiVersionsActionTest, QueryString)
{
  PiVersionsAction action("username", "password");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=PI_VERSIONS",
               action.buildQuery().c_str());
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
}
