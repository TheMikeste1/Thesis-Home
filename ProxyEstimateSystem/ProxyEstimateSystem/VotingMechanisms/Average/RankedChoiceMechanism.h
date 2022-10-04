#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>

#include <map>

#include "../VotingMechanism.h"

class RankedChoiceMechanism : VotingMechanism
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

      // Start by removing proxies that have no votes
      int i = 0;
      for (; i < proxies.size(); i++)
      {
         auto* proxy = proxies[i];
         if (votes->find(proxy) == votes->end())
         {
            weights[proxy] = i + 1;
            systemWeight += i + 1;
            _removeAgentFromRankings(proxy, remainingRankings);
         }
      }


      // Now calculate the ranking for the remaining proxies
      i++; // +1 to account for the removed proxies
      for (; i <= proxies.size(); i++)
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