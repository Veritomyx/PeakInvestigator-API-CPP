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

#include <string>
#include <PeakInvestigator/PeakInvestigatorSaaS.h>
#include <PeakInvestigator/Actions/PiVersionsAction.h>
#include <PeakInvestigator/Actions/SftpAction.h>

#include "PeakInvestigatorSaaSTest_config.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace Veritomyx::PeakInvestigator;
using ::testing::Return;

#ifdef TEST_PEAKINVESTIGATOR_WEB
TEST(PeakInvestigatorSaaS, executeAction_Ok)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  PiVersionsAction action(PI_USERNAME, PI_PASSWORD);

  std::string response = service.executeAction(&action);
  action.processResponse(response);

  ASSERT_TRUE(action.isReady("PI_VERSIONS"));
  ASSERT_FALSE(action.hasError());
}
#endif

TEST(PeakInvestigatorSaas, executeAction_BadCredentials)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  PiVersionsAction action("username", "password");

  std::string response = service.executeAction(&action);
  action.processResponse(response);

  ASSERT_TRUE(action.isReady("PI_VERSIONS"));
  ASSERT_TRUE(action.hasError());
  ASSERT_EQ(3, action.getErrorCode());
}

class MockSftpAction : public SftpAction
{
public:
    MockSftpAction() : SftpAction("username", "password", 1234) {}

    MOCK_METHOD0(getHost, std::string());
    MOCK_METHOD0(getPort, int());
    MOCK_METHOD0(getDirectory, std::string());
    MOCK_METHOD0(getSftpUsername, std::string());
    MOCK_METHOD0(getSftpPassword, std::string());
};

#ifdef TEST_PEAKINVESTIGATOR_SFTP
TEST(PeakInvestigatorSaaS, uploadFile_OK)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  MockSftpAction action;

  EXPECT_CALL(action, getHost())
      .WillOnce(Return("peakinvestigator.veritomyx.com"));
  EXPECT_CALL(action, getSftpUsername())
      .WillRepeatedly(Return(SFTP_USERNAME));
  EXPECT_CALL(action, getSftpPassword())
      .WillRepeatedly(Return(SFTP_PASSWORD));
  EXPECT_CALL(action, getPort())
      .WillOnce(Return(SFTP_PORT));

  ASSERT_NO_THROW(service.uploadFile(action, "test.tar", "test.tar"));
}
#endif

TEST(PeakInvestigatorSaaS, uploadFile_BadHost)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  MockSftpAction action;

  EXPECT_CALL(action, getHost())
      .WillOnce(Return("peakinvestigator"));
  EXPECT_CALL(action, getPort())
      .WillOnce(Return(22));

  ASSERT_THROW(service.uploadFile(action, "test.tar", "test.tar"),
               std::runtime_error);
}

TEST(PeakInvestigatorSaaS, uploadFile_BadPort)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  MockSftpAction action;

  EXPECT_CALL(action, getHost())
      .WillOnce(Return("peakinvestigator.veritomyx.com"));
  EXPECT_CALL(action, getPort())
      .WillOnce(Return(22));

  ASSERT_THROW(service.uploadFile(action, "test.tar", "test.tar"),
               std::runtime_error);
}

#ifdef TEST_PEAKINVESTIGATOR_SFTP
TEST(PeakInvestigatorSaaS, uploadFile_BadCredentials)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  MockSftpAction action;

  EXPECT_CALL(action, getHost())
      .WillOnce(Return("peakinvestigator.veritomyx.com"));
  EXPECT_CALL(action, getSftpUsername())
      .WillRepeatedly(Return("BadUsername"));
  EXPECT_CALL(action, getSftpPassword())
      .WillRepeatedly(Return("BadPassword"));
  EXPECT_CALL(action, getPort())
      .WillOnce(Return(SFTP_PORT));

  ASSERT_THROW(service.uploadFile(action, "test.tar", "test.tar"),
               std::runtime_error);
}
#endif
