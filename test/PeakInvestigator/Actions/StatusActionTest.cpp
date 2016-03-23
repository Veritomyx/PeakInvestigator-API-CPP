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
