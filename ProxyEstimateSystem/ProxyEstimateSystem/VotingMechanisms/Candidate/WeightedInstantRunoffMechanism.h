#pragma once

#include <algorithm>

#include <map>

#include "../VotingMechanism.h"

namespace candidate
{
/*
 *  Uses instant runoff until some proxy has the majority, then averages
 *  using the votes as weights.
 *  The total weight in a ranking serves as the number of votes for the
 *  ranking's first choice.
 */
   class WeightedInstantRunoffMechanism : public VotingMechanism
   {
   private:
      static PartitionedWeights* _sumInactiveWeights(
            const std::map<InactiveVoter*, Rankings>& rankings
      )
      {
         auto* ret = new PartitionedWeights();

         // Iterate through the Rankings, summing the weight for each proxy
         // and incrementing the system weight.
         for (auto& [inactiveVoter, ranking]: rankings)
         {
            auto* voter = (TruthEstimator*) (inactiveVoter);
            for (auto& rank: ranking)
            {
               ret->systemWeight += rank.weight;
               ret->weights[voter] += rank.weight;
            }
         }

         return ret;
      }

   public:
      double solve(
            const std::vector<TruthEstimator*>& proxies,
            const std::vector<InactiveVoter*>& inactive,
            const std::map<InactiveVoter*, Rankings>& rankings
      ) const override
      {
         // Get the weights for each inactive voter
         // so we can apply it to their first choice
         auto* partitionedWeights = _sumInactiveWeights(rankings);
         double systemWeight = partitionedWeights->systemWeight + proxies.size();

         // Calculate the weight for each proxy
         std::map<TruthEstimator*, double> votes;
         for (const auto proxy : proxies)
         {
            votes[proxy] = 1;
         }
         for (auto& [inactiveVoter, ranking]: rankings)
         {
            auto* voter = (TruthEstimator*) (inactiveVoter);
            auto* proxy = ranking.agentRanked(1);
            votes[proxy] += partitionedWeights->weights[voter];
         }

         TruthEstimator* maxVoteProxy = std::max_element(
               votes.begin(), votes.end(),
               [](const auto& a, const auto& b) {
                  return a.second < b.second;
               }
         )->first;

         // Run instant runoff until a proxy has the majority of the weight
         std::map<InactiveVoter*, Rankings> remainingRankings(rankings);
         while (votes.at(maxVoteProxy) < systemWeight / 2)
         {
            TruthEstimator* minVoteProxy = std::min_element(
                  votes.begin(), votes.end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
            // Remove the proxy with the least votes
            for (auto& [inactiveVoter, ranking]: remainingRankings)
            {
               int removedRank = ranking.rankFor(minVoteProxy);
               ranking.removeRanking(removedRank);
               // If this was the first choice,
               // push their weight to their next choice
               if (removedRank == 1)
               {
                  auto* voter = (TruthEstimator*) (inactiveVoter);
                  auto* newChoiceAgent = ranking.agentRanked(1);
                  votes[newChoiceAgent] += partitionedWeights->weights.at(voter);
               }
            }
            votes.erase(minVoteProxy);


            // Recount the votes
            maxVoteProxy = std::max_element(
                  votes.begin(), votes.end(),
                  [](const auto& a, const auto& b) {
                     return a.second < b.second;
                  }
            )->first;
         }

         delete partitionedWeights;
         return maxVoteProxy->getLastEstimate();
      }
   };
}
