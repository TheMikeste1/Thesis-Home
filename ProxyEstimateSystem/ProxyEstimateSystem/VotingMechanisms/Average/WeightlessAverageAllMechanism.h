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
   )
   {
      double proxySum = std::accumulate(proxies.begin(), proxies.end(), 0);
      double inactiveSum = std::accumulate(inactive.begin(), inactive.end(), 0);

      return (proxySum + inactiveSum) / (proxies.size + inactive.size());
   }
};
