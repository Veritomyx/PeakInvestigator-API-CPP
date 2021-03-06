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

#ifndef PEAKINVESTIGATORSAAS_H
#define PEAKINVESTIGATORSAAS_H

#include <string>
#include <fstream>

#include "PeakInvestigatorSaaS_export.h"
#include "AbstractProgress.h"

struct _LIBSSH2_SESSION;
struct _LIBSSH2_SFTP;
struct _LIBSSH2_SFTP_HANDLE;

namespace Veritomyx
{
  namespace PeakInvestigator
  {

    class BaseAction;
    class SftpAction;

    enum Direction { UPLOAD, DOWNLOAD };

    class PEAKINVESTIGATORSAAS_EXPORT PeakInvestigatorSaaS
    {
      public:

        PeakInvestigatorSaaS(std::string hostname, std::string path = "/api/", std::string agent = "libcurl-agent/1.0");
        ~PeakInvestigatorSaaS();

        void setDebug(bool debug = true);

        std::string executeAction(BaseAction* action);

        void uploadFile(SftpAction& action, std::string localFilename, std::string remoteFilename, AbstractProgress* progress = NULL);
        void downloadFile(SftpAction& action, std::string remoteFilename, std::string localFilename, AbstractProgress* progress = NULL);

      private:

        void establishSSHSession_(SftpAction &action);
        void confirmSSHServerIdentity_(SftpAction &action);
        void authenticateUser_(SftpAction &action);
        void establishSFTPSession_();
        void disconnect_();

        int getConnectedSocket(const char* host, const char* port);
        _LIBSSH2_SFTP_HANDLE* getSftpHandle(_LIBSSH2_SFTP* sftp_session, Direction direction, const char* filename);

        void uploadFile_(std::ifstream& file, _LIBSSH2_SFTP_HANDLE* sftp, AbstractProgress* progress);
        void downloadFile_(std::ofstream& file, _LIBSSH2_SFTP_HANDLE* sftp, AbstractProgress* progress);

        std::string hostname_;
        std::string path_;
        std::string agent_;

        int state_;
        int socket_;
        struct addrinfo* host_info_;

        _LIBSSH2_SESSION* ssh_session_;
        _LIBSSH2_SFTP* sftp_session_;
    };

  }
}

#endif // PEAKINVESTIGATORSAAS_H
