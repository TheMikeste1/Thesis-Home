#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include <map>

#include "../VotingMechanism.h"

/*
 * Ignores weights. Uses instant runoff to eliminate proxies until some
 * proxy has the majority, then averages using the votes as weights.
 */
class InstantRunoffMechanism : VotingMechanism
{
private:
   std::map<TruthEstimator*, int>* _countVotes(const std::map<InactiveVoter*, Rankings>& rankings)
   {
      std::map<TruthEstimator*, int>* ret = new std::map<TruthEstimator*, int>();

      for (const auto& [_, ranking] : rankings)
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
      float totalVotes = inactive.size();
      auto* votes = _countVotes(rankings);
      TruthEstimator* maxVoteProxy = std::max(votes->begin(), votes->end(), 
         [](const auto& a, const auto& b) { return a.second < b.second; }
      )->first;


      std::map<InactiveVoter*, Rankings> remainingRankings(rankings);
      while (votes[maxVoteProxy] < totalVotes / 2)
      {
         TruthEstimator* minVoteProxy = std::min(votes->begin(), votes->end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }
         )->first;
         // Remove the proxy with the least votes
         for (auto& [_, ranking] : remainingRankings)
            ranking.removeAgent(minVoteProxy);
         // Recount the votes
         delete votes;
         votes = _countVotes(remainingRankings);
         maxVoteProxy = std::max(votes->begin(), votes->end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }
         )->first;
      }

      std::vector<double> appliedWeights;
      std::ranges::transform(
         votes,
         std::back_inserter(appliedWeights),
         [](const auto& pair)
         {
            auto* agent = pair.first;
            double weight = pair.second;
            return agent->lastEstimate * weight;
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