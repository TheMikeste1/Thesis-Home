#pragma once

#include <map>

#include "../InactiveVoter.h"
#include "../Rankings.h"

class VotingMechanism
{
protected:
   struct PartitionedWeights
   {
      double systemWeight = 0;
      std::map<TruthEstimator*, double> weights;
   };

   [[nodiscard]] static std::map<TruthEstimator*, int>* _countVotes(
         const std::map<InactiveVoter*, Rankings>& rankings
   )
   {
      auto* ret = new std::map<TruthEstimator*, int>();

      for (const auto item : rankings.begin()->second)
      {
         (*ret)[item.proxy] = 1;
      }

      for (const auto& [_, ranking]: rankings)
      {
         auto* estimator = ranking.agentRanked(1);
         if (ret->find(estimator) == ret->end())
            (*ret)[estimator] = 1;
         (*ret)[estimator] += 1;
      }

      return ret;
   }

   static PartitionedWeights* _sumProxyWeights(
         const std::map<InactiveVoter*, Rankings>& rankings
   )
   {
      auto* ret = new PartitionedWeights();
      ret->systemWeight = double(rankings.begin()->second.size());
      // Iterate through the Rankings, summing the weight for each proxy
      // and incrementing the system weight.
      for (auto& [_, ranking]: rankings)
      {
         for (auto& rank: ranking)
         {
            ret->systemWeight += rank.weight;
            if (ret->weights.find(rank.proxy) == ret->weights.end())
            {
               ret->weights[rank.proxy] = 1;
            }
            ret->weights[rank.proxy] += rank.weight;
         }
      }

      return ret;
   }

public:
   virtual ~VotingMechanism() = default;

   virtual double solve(
         const std::vector<TruthEstimator*>& proxies,
         const std::vector<InactiveVoter*>& inactive,
         const std::map<InactiveVoter*, Rankings>& rankings
   ) const = 0;
};
