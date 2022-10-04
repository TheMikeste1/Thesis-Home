#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include "../VotingMechanism.h"

class MeanMechanism : VotingMechanism
{
public:
   double solve(
         const std::vector<TruthEstimator*>& proxies,
         const std::vector<InactiveVoter*>& inactive,
         const std::map<InactiveVoter*, Rankings>& rankings
   ) const override
   {
      auto* weights = _sumProxyWeights(rankings);

      const double systemWeight = weights->systemWeight;

      std::vector<double> appliedWeights;
      std::ranges::transform(
            weights->proxyWeights,
            std::back_inserter(appliedWeights),
            [](const auto& pair) {
               auto* agent = pair.first;
               double weight = pair.second;
               return agent->getLastEstimate() * weight;
            }
      );
      delete weights;

      return std::accumulate(
            appliedWeights.begin(),
            appliedWeights.end(),
            static_cast<double>(0)
      ) / systemWeight;
   }
};