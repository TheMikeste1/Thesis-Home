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
      double totalWeight = 0;
      auto it = orderedProxies.begin();
      while (it != orderedProxies.end())
      {
         double distance = abs(agent->getLastEstimate() - (*it)->getLastEstimate());
         totalWeight += distance;
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

      // This will give us the total distances from the max distance for all proxies.
      // This is equivalent to summing the differences between the max distance and the
      // distance for each proxy.
      // maxDistance * orderedProxies.size() gives us the total distance if all proxies
      // were at the max distance, then we can subtract the total actual distance to get
      // the total distance from the max distance.
      totalWeight = (maxDistance * orderedProxies.size()) - totalWeight;

      // Add all the proxies with the difference from maxDistance as weight
      it = orderedProxies.begin();
      while (it != orderedProxies.end())
      {
         double distance = distances[*it];
         ret.insert(*it, (maxDistance - distance) / totalWeight);
         ++it;
      }

      return ret;
   }
};
