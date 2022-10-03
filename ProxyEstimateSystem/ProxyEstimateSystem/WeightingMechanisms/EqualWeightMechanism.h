#pragma once

#include "WeightingMechanism.h"

class EqualWeightMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*>& orderedProxies) override
   {
      auto ret = Rankings();

      while (++it != orderedProxies.end())
      {
         ret.insert(*it, 1);
      }

      return ret;
   }
};
