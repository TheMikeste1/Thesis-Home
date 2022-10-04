#pragma once

#include <numeric>

#include "../VotingMechanism.h"

class WeightlessAverageAllMechanism : VotingMechanism
{
public:
   double solve(
         const std::vector<TruthEstimator*>& proxies,
         const std::vector<InactiveVoter*>& inactive,
         const std::map<InactiveVoter*, Rankings>& rankings
   ) const override
   {
      double proxySum = std::accumulate(
            proxies.begin(), proxies.end(), 0,
            [](double sum, TruthEstimator* proxy) {
               return sum + proxy->getLastEstimate();
            });
      double inactiveSum = std::accumulate(
            inactive.begin(), inactive.end(), 0,
            [](double sum, InactiveVoter* voter) {
               return sum + voter->getLastEstimate();
            });

      return (proxySum + inactiveSum) / double(proxies.size() + inactive.size());
   }
};
