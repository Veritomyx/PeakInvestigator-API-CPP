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

#include <PeakInvestigator/Actions/SftpAction.h>
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(SftpActionTest, QueryString)
{
  SftpAction action("username", "password", 1234);
  SftpAction action2(action);

  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=SFTP&ID=1234",
               action.buildQuery().c_str());
  ASSERT_STREQ("Version=5.4&User=username&Code=password&Action=SFTP&ID=1234",
               action2.buildQuery().c_str());
}

TEST(SftpActionTest, ExampleResponse)
{
  SftpAction action("username", "password", 1234);
  action.processResponse(SftpAction::EXAMPLE_RESPONSE);

  ASSERT_STREQ("peakinvestigator.veritomyx.com", action.getHost().c_str());
  ASSERT_EQ(22022, action.getPort());
  ASSERT_STREQ("/files", action.getDirectory().c_str());
  ASSERT_STREQ("Vt504", action.getSftpUsername().c_str());
  ASSERT_STREQ("0UtnWMvzoi2jF4BQ", action.getSftpPassword().c_str());

  SftpFingerprints fingerprints = action.getFingerprints();
  ASSERT_STREQ("96:bd:da:62:5a:53:1a:2f:82:87:65:7f:c0:45:71:94", fingerprints.getHash("DSA-MD5").c_str());
  ASSERT_STREQ("b9SOs40umHMywBa2GtdsOhr/wgP1L6nfXWugjRrJTaM", fingerprints.getHash("DSA-SHA256").c_str());

  SftpAction action2(action);

  ASSERT_STREQ("peakinvestigator.veritomyx.com", action2.getHost().c_str());
  ASSERT_EQ(22022, action2.getPort());
  ASSERT_STREQ("/files", action2.getDirectory().c_str());
  ASSERT_STREQ("Vt504", action2.getSftpUsername().c_str());
  ASSERT_STREQ("0UtnWMvzoi2jF4BQ", action2.getSftpPassword().c_str());

  fingerprints = action2.getFingerprints();
  ASSERT_STREQ("96:bd:da:62:5a:53:1a:2f:82:87:65:7f:c0:45:71:94", fingerprints.getHash("DSA-MD5").c_str());
  ASSERT_STREQ("b9SOs40umHMywBa2GtdsOhr/wgP1L6nfXWugjRrJTaM", fingerprints.getHash("DSA-SHA256").c_str());
}
