#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include <map>

#include "../VotingMechanism.h"
namespace average
{
   class RankedChoiceMechanism : public VotingMechanism
   {
   private:
      static void _removeAgentFromRankings(
            TruthEstimator* const agent,
            std::map<InactiveVoter*, Rankings>& rankings
      )
      {
         for (auto& [_, ranking]: rankings)
            ranking.removeAgent(agent);
      }

   public:
      double solve(
            const std::vector<TruthEstimator*>& proxies,
            const std::vector<InactiveVoter*>& inactive,
            const std::map<InactiveVoter*, Rankings>& rankings
      ) const override
      {
         std::map<TruthEstimator*, int> weights;
         std::map<TruthEstimator*, int>* votes = _countVotes(rankings);
         int systemWeight = 0;
         std::map<InactiveVoter*, Rankings> remainingRankings(rankings);

         // Now calculate the ranking for the remaining proxies
         // Start counting at 1 so every proxy gets a vote
         for (int i = 1; i <= proxies.size(); i++)
         {
            delete votes;
            votes = _countVotes(remainingRankings);
            auto* minVoteProxy = std::min_element(
                  votes->begin(),
                  votes->end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
            weights[minVoteProxy] = i;
            systemWeight += i;
            _removeAgentFromRankings(minVoteProxy, remainingRankings);
         }

         delete votes;

         // Calculate the weighted average
         std::vector<double> appliedWeights;
         std::transform(
               weights.begin(), weights.end(),
               std::back_inserter(appliedWeights),
               [](const auto& pair) {
                  auto* agent = pair.first;
                  double weight = pair.second;
                  return agent->getLastEstimate() * weight;
               }
         );

         return std::accumulate(
               appliedWeights.begin(),
               appliedWeights.end(),
               static_cast<double>(0)
         ) / systemWeight;
      }
   };
}