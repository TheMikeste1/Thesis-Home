#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include <map>

#include "../VotingMechanism.h"

namespace average
{
/*
 * Ignores weights. Uses instant runoff to eliminate proxies until some
 * proxy has the majority, then averages using the votes as weights.
 */
   class InstantRunoffMechanism : public VotingMechanism
   {
   public:
      double solve(
            const std::vector<TruthEstimator*>& proxies,
            const std::vector<InactiveVoter*>& inactive,
            const std::map<InactiveVoter*, Rankings>& rankings
      ) const override
      {
         auto totalVotes = float(inactive.size());
         std::map<TruthEstimator*, int>* votes = _countVotes(rankings);
         TruthEstimator* maxVoteProxy = std::max_element(
               votes->begin(), votes->end(),
               [](const auto& a, const auto& b) {
                  return a.second < b.second;
               }
         )->first;

         std::map<InactiveVoter*, Rankings> remainingRankings(rankings);
         while (float(votes->at(maxVoteProxy)) < totalVotes / 2)
         {
            TruthEstimator* minVoteProxy = std::min_element(
                  votes->begin(), votes->end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
            // Remove the proxy with the least votes
            for (auto& [_, ranking]: remainingRankings)
               ranking.removeAgent(minVoteProxy);
            // Recount the votes
            delete votes;
            votes = _countVotes(remainingRankings);
            maxVoteProxy = std::max_element(
                  votes->begin(), votes->end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
         }

         std::vector<double> appliedWeights;
         std::transform(
               votes->begin(), votes->end(),
               std::back_inserter(appliedWeights),
               [](const auto& pair) {
                  auto* agent = pair.first;
                  double weight = pair.second;
                  return agent->getLastEstimate() * weight;
               }
         );
         delete votes;
         return std::accumulate(
               appliedWeights.begin(),
               appliedWeights.end(),
               static_cast<double>(0)
         ) / totalVotes;
      }
   };
}
