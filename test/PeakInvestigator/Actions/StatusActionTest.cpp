#include <ctime>

#include <PeakInvestigator/Actions/StatusAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

char* format_datetime(struct tm& datetime) {
  int size = sizeof("yyyy-mm-dd HH:MM:SS");
  char* buffer = new char[size];
  int retval = strftime(buffer, size, BaseAction::PARSE_DATE_FORMAT.c_str(), &datetime);
  return buffer;
}

TEST(StatusActionTest, QueryString)
{
  StatusAction action("username", "password", "P-1234");

  ASSERT_STREQ("Version=3.4&User=username&Code=password&Action=STATUS&Job=P-1234",
               action.buildQuery().c_str());
}

// "{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Preparing\", \"Datetime\":\"2016-02-03 18:18:12\"}"
TEST(StatusActionTest, ExampleResponse_Preparing)
{
  StatusAction action("username", "password", "P-1234");
  action.processResponse(StatusAction::EXAMPLE_RESPONSE_PREPARING);

  ASSERT_STREQ("P-504.5148", action.getJob().c_str());
  ASSERT_EQ(StatusAction::PREPARING, action.getStatus());

  struct tm datetime = action.getDateTime();
  char* buffer = format_datetime(datetime);
  ASSERT_STREQ("2016-02-03 18:18:12", buffer);

  delete buffer;
}

// "{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Running\", \"Datetime\":\"2016-02-03 18:25:09\"}"
TEST(StatusActionTest, ExampleResponse_Running)
{
  StatusAction action("username", "password", "P-1234");
  action.processResponse(StatusAction::EXAMPLE_RESPONSE_RUNNING);

  ASSERT_STREQ("P-504.5148", action.getJob().c_str());
  ASSERT_EQ(StatusAction::RUNNING, action.getStatus());

  struct tm datetime = action.getDateTime();
  char* buffer = format_datetime(datetime);
  ASSERT_STREQ("2016-02-03 18:25:09", buffer);

  delete buffer;
}

// "{\"Action\":\"STATUS\", \"Job\":\"P-504.5148\", \"Status\":\"Done\", \"Datetime\":\"2016-02-03 18:31:05\", \"ScansInput\":3, \"ScansComplete\":3, \"ActualCost\":0.36,
//                                     \"JobLogFile\":\"\\/files\\/P-504.5148\\/P-504.5148.log.txt\", \"ResultsFile\":\"\\/files\\/P-504.5148\\/P-504.5148.mass_list.tar\"}"
TEST(StatusActionTest, ExampleResponse_Done)
{
  StatusAction action("username", "password", "P-1234");
  action.processResponse(StatusAction::EXAMPLE_RESPONSE_DONE);

  ASSERT_STREQ("P-504.5148", action.getJob().c_str());
  ASSERT_EQ(StatusAction::DONE, action.getStatus());

  struct tm datetime = action.getDateTime();
  char* buffer = format_datetime(datetime);
  ASSERT_STREQ("2016-02-03 18:31:05", buffer);

  delete buffer;

  ASSERT_EQ(3, action.getNumberOfInputScans());
  ASSERT_EQ(3, action.getNumberOfCompleteScans());
  ASSERT_EQ(0.36, action.getActualCost());
  ASSERT_STREQ("/files/P-504.5148/P-504.5148.log.txt", action.getLogFilename().c_str());
  ASSERT_STREQ("/files/P-504.5148/P-504.5148.mass_list.tar", action.getResultsFilename().c_str());
}

// "{\"Action\":\"STATUS\", \"Job\":\"P-504.1463\", \"Status\":\"Deleted\", \"Datetime\":\"2016-02-03 18:36:05\"}"
TEST(StatusActionTest, ExampleResponse_Deleted)
{
  StatusAction action("username", "password", "P-1234");
  action.processResponse(StatusAction::EXAMPLE_RESPONSE_DELETED);

  ASSERT_STREQ("P-504.1463", action.getJob().c_str());
  ASSERT_EQ(StatusAction::DELETED, action.getStatus());

  struct tm datetime = action.getDateTime();
  char* buffer = format_datetime(datetime);
  ASSERT_STREQ("2016-02-03 18:36:05", buffer);

  delete buffer;
}
