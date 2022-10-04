#pragma once

#include "WeightingMechanism.h"

class BordaMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*>& orderedProxies) override
   {
      auto ret = Rankings();

      auto it = orderedProxies.rbegin();
      for (int i = 1; it != orderedProxies.rend(); i++, ++it)
      {
         ret.insert(*it, i);
      }

      return ret;
   }
};
