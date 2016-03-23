#include <ctime>

#include <PeakInvestigator/Actions/DeleteAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(DeleteActionTest, QueryString)
{
  DeleteAction action("username", "password", "P-1234");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=DELETE&Job=P-1234",
               action.buildQuery().c_str());
}

// "{\"Action\":\"DELETE\",\"Job\":\"P-504.4256\",\"Datetime\":\"2016-02-03 18:35:06\"}"

TEST(DeleteActionTest, ExampleResponse)
{
  DeleteAction action("username", "password", "P-1234");
  action.processResponse(DeleteAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("P-504.4256", action.getJob().c_str());

  struct tm datetime = action.getDateTime();
  int size = sizeof("yyyy-mm-dd HH:MM:SS");
  char buffer[size];
  int retval = strftime(buffer, size, BaseAction::PARSE_DATE_FORMAT.c_str(), &datetime);
  ASSERT_EQ(size, retval + 1);
  ASSERT_STREQ("2016-02-03 18:35:06", buffer);
}
