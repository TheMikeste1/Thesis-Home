#pragma once

#include <algorithm>
#include <map>
#include <stdexcept>

#include "../VotingMechanism.h"

class MedianMechanism : public VotingMechanism
{
public:
   double solve(
         const std::vector<TruthEstimator*>& proxies,
         const std::vector<InactiveVoter*>& inactive,
         const std::map<InactiveVoter*, Rankings>& rankings
   ) const override
   {
      auto* partitionedWeights = _sumProxyWeights(rankings);

      std::vector<TruthEstimator*> sortedProxies(proxies);
      std::sort(
            sortedProxies.begin(),
            sortedProxies.end(),
            [](const auto& a, const auto& b) {
               return a->getLastEstimate() < b->getLastEstimate();
            }
      );
      double sum = 0;
      for (const auto& proxy: sortedProxies)
      {
         sum += partitionedWeights->weights.at(proxy);
         if (sum > partitionedWeights->systemWeight / 2)
         {
            delete partitionedWeights;
            return proxy->getLastEstimate();
         }
      }
      delete partitionedWeights;
      // Should never reach here
      throw std::runtime_error("MedianMechanism::solve() failed");
   }
};
