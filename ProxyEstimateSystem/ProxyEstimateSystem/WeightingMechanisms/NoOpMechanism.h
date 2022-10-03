#pragma once

#include "WeightingMechanism.h"

class NoOpMechanism : public WeightingMechanism
{
public:
   Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies) override
   {
      return Rankings();
   }
};
