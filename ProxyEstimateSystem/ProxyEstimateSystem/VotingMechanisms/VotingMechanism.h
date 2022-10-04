/*
from __future__ import annotations

from abc import ABC, abstractmethod
from collections import defaultdict
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class VotingMechanism(ABC):
    @abstractmethod
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        pass

    @staticmethod
    def _sum_proxy_weights(rankings: dict[InactiveVoter, Rankings]) \
            -> (float, float):
        proxy_weights = defaultdict(float)

        system_weight = 0
        for ranking in rankings.values():
            for rank in ranking:
                system_weight += rank.weight
                proxy_weights[rank.proxy] += rank.weight

        return proxy_weights, system_weight
*/
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
