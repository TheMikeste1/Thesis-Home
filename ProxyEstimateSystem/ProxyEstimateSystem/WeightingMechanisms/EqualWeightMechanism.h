#pragma once

#include "WeightingMechanism.h"

class EqualWeightMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent,
                          std::vector<TruthEstimator*>& orderedProxies) override
   {
      auto ret = Rankings();

      for (auto proxy: orderedProxies)
      {
         ret.insert(proxy, 1);
      }

      return ret;
   }
};
