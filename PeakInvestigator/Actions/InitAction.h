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

#ifndef INIT_ACTION_H
#define INIT_ACTION_H

#include <string>
#include <map>
#include <list>

#include <PeakInvestigator/PeakInvestigatorSaaS_export.h>
#include "BaseAction.h"

namespace Veritomyx
{

  namespace PeakInvestigator
  {

    class PEAKINVESTIGATORSAAS_EXPORT ResponseTimeCosts : public std::map<std::string, double>
    {

      public:
        std::list<std::string> getRTOs() const;
        double getCost(std::string responseTimeObjective) const;
    };

    class PEAKINVESTIGATORSAAS_EXPORT EstimatedCosts : public std::map<std::string, ResponseTimeCosts>
    {
      public:
        std::list<std::string> getInstruments() const;
        double getMaximumCost(std::string RTO) const;

        ResponseTimeCosts forInstrument(std::string) const;
    };

    struct PEAKINVESTIGATORSAAS_EXPORT JobAttributes
    {
        int max_points;
        int min_mass;
        int max_mass;
        int start_mass;
        int end_mass;
    };

    class PEAKINVESTIGATORSAAS_EXPORT InitAction : public BaseAction
    {
      private:

        int project_id_;
        std::string version_of_PI_;
        int scan_count_;
        int calibration_count_;
        JobAttributes attributes_;
        std::string client_key_;
        EstimatedCosts estimated_costs_;

      public:
        static const std::string EXAMPLE_RESPONSE;

        InitAction(std::string user, std::string code,
                   int project_id, std::string version_of_PI, int scan_count,
                   const JobAttributes& attributes, int calibration_count = 0, std::string client_key = "");

        std::string buildQuery() const;

        void preCheck();

        std::string getJob();

        long getProjectId();

        double getFunds();

        EstimatedCosts getEstimatedCosts();

        std::string getErrorMessage();

        int getErrorCode();
    };

  }

}

#endif // INIT_ACTION_H
