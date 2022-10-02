#pragma once

#include <vector>
#include "../Rankings.h"

class WeigtingMechanism
{
public:
   virtual Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies) = 0;
};
