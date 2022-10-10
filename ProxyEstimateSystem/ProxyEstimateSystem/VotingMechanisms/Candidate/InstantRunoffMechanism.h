#pragma once

#include <map>

#include "../VotingMechanism.h"

namespace candidate
{
/*
 * Ignores weights. Uses instant runoff to eliminate proxies until some
 * proxy has the majority, then returns its vote.
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
         auto totalVotes = float(inactive.size() + proxies.size());
         std::map<TruthEstimator*, int>* votes = _countVotes(rankings);
         TruthEstimator* maxVoteProxy = std::max_element(
               votes->begin(), votes->end(),
               [](const auto& a, const auto& b) {
                  return a.second < b.second;
               }
         )->first;

         std::map<InactiveVoter*, Rankings> remainingRankings(rankings);
         while (double(votes->at(maxVoteProxy)) < totalVotes / 2)
         {
            TruthEstimator* minVoteProxy = std::min_element(
                  votes->begin(), votes->end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
            totalVotes--;
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

         delete votes;
         return maxVoteProxy->getLastEstimate();
      }
   };
}
