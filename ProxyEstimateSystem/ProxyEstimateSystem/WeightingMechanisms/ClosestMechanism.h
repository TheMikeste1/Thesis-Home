#pragma once

#include "WeightingMechanism.h"

class ClosestMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent,
                          std::vector<TruthEstimator*>& orderedProxies) override
   {
      auto ret = Rankings();
      auto it = orderedProxies.begin();
      ret.insert(*it, 1);
      while (++it != orderedProxies.end())
      {
         ret.insert(*it, 0);
      }

      return ret;
   }
};
