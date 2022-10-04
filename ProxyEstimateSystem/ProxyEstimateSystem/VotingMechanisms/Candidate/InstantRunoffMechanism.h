#pragma once

#include <map>

#include "../VotingMechanism.h"

/*
 * Ignores weights. Uses instant runoff to eliminate proxies until some
 * proxy has the majority, then returns its vote.
 */
class InstantRunoffMechanism : VotingMechanism
{
private:
   [[nodiscard]] static std::map<TruthEstimator*, int>*
   _countVotes(const std::map<InactiveVoter*, Rankings>& rankings)
   {
      auto* ret = new std::map<TruthEstimator*, int>();

      for (const auto& [_, ranking]: rankings)
      {
         auto* estimator = ranking.agentRanked(1);
         if (ret->find(estimator) != ret->end())
            (*ret)[estimator] = 0;
         (*ret)[estimator] += 1;
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

      delete votes;
      return maxVoteProxy->getLastEstimate();
   }
};