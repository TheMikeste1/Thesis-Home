#pragma once

#include "WeightingMechanism.h"

class NoOpMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent,
                          std::vector<TruthEstimator*>& orderedProxies) override
   {
      return {};
   }
};
