#pragma once

#include <vector>
#include "../Rankings.h"

class WeightingMechanism
{
public:
   virtual Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies) = 0;
};
