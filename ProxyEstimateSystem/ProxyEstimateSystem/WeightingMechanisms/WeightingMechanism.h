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
   virtual ~WeightingMechanism() = default;

   Rankings applyWeights(TruthEstimator* agent, std::vector<TruthEstimator*> proxies)
   {
      if (proxies.empty())
      {
         return {};
      }

      // Sort the proxies by position
      std::sort(
            proxies.begin(),
            proxies.end(),
            [agent](TruthEstimator* a, TruthEstimator* b) {
               return abs(agent->getLastEstimate() - a->getLastEstimate())
                      < abs(agent->getLastEstimate() - b->getLastEstimate());
            }
      );
      Rankings ret = this->_applyWeights(agent, proxies);
      assert(ret.size() == proxies.size());

#ifndef NDEBUG
      // Check that the weights add up to 1
      double total = 0;
      for (auto& [_, weight] : ret)
      {
         total += weight;
      }
      assert(total == 1);
#endif

      return ret;
   }
};
