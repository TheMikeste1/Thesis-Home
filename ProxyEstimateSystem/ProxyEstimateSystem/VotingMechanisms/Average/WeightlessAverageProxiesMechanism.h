#pragma once

#include <numeric>

#include "../VotingMechanism.h"

namespace average
{
   class WeightlessAverageProxiesMechanism : public VotingMechanism
   {
   public:
      double solve(
            const std::vector<TruthEstimator*>& proxies,
            const std::vector<InactiveVoter*>& inactive,
            const std::map<InactiveVoter*, Rankings>& rankings
      ) const override
      {
         double proxySum = std::accumulate(
               proxies.begin(), proxies.end(), 0.0,
               [](double sum, TruthEstimator* proxy) {
                  return sum + proxy->getLastEstimate();
               });

         return proxySum / double(proxies.size());
      }
   };
}
