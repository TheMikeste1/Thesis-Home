#pragma once

#include <map>

#include "../InactiveVoter.h"
#include "../Rankings.h"

class VotingMechanism
{
private:
   struct PartitionedWeights
   {
      double systemWeight;
      std::map<TruthEstimator*, double> proxyWeights;
   };

   static PartitionedWeights* _sumProxyWeights(
      const std::map<InactiveVoter*, Rankings>& rankings
   )
   {
      PartitionedWeights* ret = new PartitionedWeights();

      // Iterate through the Rankings, summing the weight for each proxy
      // and incrementing the system weight.
      for (auto& [inactiveVoter, ranking] : rankings)
      {
         for (auto& rank : ranking)
         {
            ret->systemWeight += rank.weight;
            ret->proxyWeights[rank.proxy] += rank.weight;
         }
      }

      return ret;
   }

public:
   double solve(
      const std::vector<TruthEstimator*>& proxies,
      const std::vector<InactiveVoter*>& inactive,
      const std::map<InactiveVoter*, Rankings>& rankings
   ) = 0;
};
