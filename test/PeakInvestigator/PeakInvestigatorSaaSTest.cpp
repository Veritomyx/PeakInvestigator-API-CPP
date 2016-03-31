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

#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(PeakInvestigatorSaaS, executeAction)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  PiVersionsAction action(PI_USERNAME, PI_PASSWORD);

  std::string response = service.executeAction(&action);
  action.processResponse(response);

  ASSERT_TRUE(action.isReady("PI_VERSIONS"));
  if(PI_USERNAME == "" || PI_PASSWORD == "")
  {
    std::cout << "Testing without username or password specified." << std::endl;
    ASSERT_TRUE(action.hasError());
  }
  else
  {
    ASSERT_FALSE(action.hasError());
  }
}

class MockSftpAction : public SftpAction
{
  public:
    MockSftpAction(std::string hostname,
                   std::string sftpUsername,
                   std::string sftpPassword,
                   int port) : SftpAction("", "", 0)
    {
      hostname_ = hostname;
      username_ = sftpUsername;
      password_ = sftpPassword;
      port_ = port;
    }

    std::string getHost() { return hostname_; }
    std::string getSftpUsername() { return username_; }
    std::string getSftpPassword() { return password_; }
    int getPort() { return port_; }
    bool preCheck() { return true; }

  private:
    std::string hostname_;
    std::string username_;
    std::string password_;
    int port_;
};

TEST(PeakInvestigatorSaaS, uploadFile_OK)
{
  PeakInvestigatorSaaS service("peakinvestigator.veritomyx.com");
  MockSftpAction action("peakinvestigator.veritomyx.com", SFTP_USERNAME,
                           SFTP_PASSWORD, SFTP_PORT);

  service.uploadFile(action, "test.tar", "test.tar");
}
