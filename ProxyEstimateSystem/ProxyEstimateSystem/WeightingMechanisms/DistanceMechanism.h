#pragma once

#include <algorithm>
#include <map>

#include "WeightingMechanism.h"

class DistanceMechanism : public WeightingMechanism
{
private:
   Rankings _applyWeights(TruthEstimator* agent,
                          std::vector<TruthEstimator*>& orderedProxies) override
   {
      auto ret = Rankings();

      // Calculate all the distances
      // Create a map to hold the distances for each truth estimator
      std::map<TruthEstimator*, double> distances;
      auto it = orderedProxies.begin();
      while (it != orderedProxies.end())
      {
         double distance = abs(agent->getLastEstimate() - (*it)->getLastEstimate());
         distances.emplace(*it, distance);
         ++it;
      }

      // Find the max distance
      double maxDistance = std::max_element(distances.begin(), distances.end(),
                                            [](const std::pair<TruthEstimator*, double>& p1,
                                               const std::pair<TruthEstimator*, double>& p2) {
                                               return p1.second < p2.second;
                                            }
      )->second;

      // Add all the proxies with the difference fron maxDistance as weight
      it = orderedProxies.begin();
      while (it != orderedProxies.end())
      {
         double distance = distances[*it];
         ret.insert(*it, maxDistance - distance);
         ++it;
      }

      return ret;
   }
};
