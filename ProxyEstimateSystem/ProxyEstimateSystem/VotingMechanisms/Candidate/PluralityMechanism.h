#pragma once

#include <map>

#include "../VotingMechanism.h"

namespace candidate
{
   class PluralityMechanism : public VotingMechanism
   {
   public:
      double solve(
            const std::vector<TruthEstimator*>& proxies,
            const std::vector<InactiveVoter*>& inactive,
            const std::map<InactiveVoter*, Rankings>& rankings
      ) const override
      {
         auto* proxyWeights = _sumProxyWeights(rankings);
         double ret = std::max_element(
               proxyWeights->weights.begin(), proxyWeights->weights.end(),
               [](const auto& lhs, const auto& rhs) {
                  return lhs.second < rhs.second;
               }
         )->first->getLastEstimate();
         delete proxyWeights;
         return ret;
      }
   };
}
