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
