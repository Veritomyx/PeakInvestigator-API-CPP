#include <PeakInvestigator/Actions/SftpAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(SftpActionTest, QueryString)
{
  SftpAction action("username", "password", 1234);

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=SFTP&ID=1234",
               action.buildQuery().c_str());
}

// "{\"Action\":\"SFTP\",\"Host\":\"peakinvestigator.veritomyx.com\",\"Port\":22022,\"Directory\":\"/files/\",\"Login\":\"SFTPUSER\",\"Password\":\"SFTPPASSWORD\"}"
TEST(SftpActionTest, ExampleResponse)
{
  SftpAction action("username", "password", 1234);
  action.processResponse(SftpAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("peakinvestigator.veritomyx.com", action.getHost().c_str());
  ASSERT_EQ(22022, action.getPort());
  ASSERT_STREQ("/files/", action.getDirectory().c_str());
  ASSERT_STREQ("SFTPUSER", action.getSftpUsername().c_str());
  ASSERT_STREQ("SFTPPASSWORD", action.getSftpPassword().c_str());
}
