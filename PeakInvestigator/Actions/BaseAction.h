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

#ifndef BASE_ACTION_H
#define BASE_ACTION_H

#include <ctime>

#include <map>
#include <string>
#include <vector>

#include <PeakInvestigator/PeakInvestigatorSaaS_export.h>

namespace Json
{
  class Value;
}

namespace Veritomyx
{

  namespace PeakInvestigator
  {

    class PEAKINVESTIGATORSAAS_EXPORT BaseAction
    {
      private:

        std::string user_;
        std::string code_;
        std::string action_;

      protected:
        Json::Value* response_object_;

      public:
        const static std::string VERSION_OF_API;
        const static std::string PARSE_DATE_FORMAT;

        BaseAction(std::string user, std::string code, std::string action);

        virtual ~BaseAction();

        virtual std::string buildQuery() const;

        void processResponse(const std::string response);

        bool isReady(std::string action);

        bool hasError();

        virtual std::string getErrorMessage(void);

        virtual int getErrorCode(void);

        std::string getStringAttribute(std::string attribute) const;

        int getIntAttribute(std::string attribute) const;

        long getLongAttribute(std::string attribute) const;

        double getDoubleAttribute(std::string attribute) const;

#ifndef _WIN32
        struct tm getDateTimeAttribute(std::string attribute = "Datetime") const;
#endif

        std::vector<std::string> getStringArrayAttribute(std::string attribute) const;

    };

  }

}

#endif // BASE_ACTION_H
