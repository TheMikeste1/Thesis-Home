#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "../Rankings.h"

class WeightingMechanism
{
private:
   virtual Rankings _applyWeights(
      TruthEstimator* agent,
      std::vector<TruthEstimator*>& orderedProxies
   ) = 0;
public:
   Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies)
   {
      if (proxies.empty())
      {
         return {};
      }

      // Sort the proxies by position
      std::ranges::sort(
         proxies,
         [agent](TruthEstimator* a, TruthEstimator* b)
         {
            return abs(agent->getLastEstimate() - a->getLastEstimate())
               < abs(agent->getLastEstimate() - b->getLastEstimate());
         }
      );
      Rankings ret = this->_applyWeights(agent, proxies);
      assert(ret.size() == proxies.size());
      return ret;
   }
};
