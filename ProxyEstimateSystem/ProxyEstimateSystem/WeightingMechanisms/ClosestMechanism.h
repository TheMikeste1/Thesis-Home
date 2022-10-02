#pragma once

#include <algorithm>

#include "WeightingMechanism.h"

class ClosestMechanism : public WeightingMechanism
{
public:
   Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies) override
   {
      auto ret = Rankings();

      if (proxies.empty())
      {
         return ret;
      }

      // Sort the proxies by position
      std::sort(
         proxies.begin(),
         proxies.end(),
         [agent](TruthEstimator* a, TruthEstimator* b) {
            return abs(agent->lastEstimate - a->lastEstimate)
               < abs(agent->lastEstimate - b->lastEstimate);
         }
      );

      auto it = proxies.begin();
      ret.insert(*it, 1);
      while (++it != proxies.end())
      {
         ret.insert(*it, 0);
      }

      return ret;
   }
};
