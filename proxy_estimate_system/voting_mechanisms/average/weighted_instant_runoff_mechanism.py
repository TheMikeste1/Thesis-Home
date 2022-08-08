from __future__ import annotations

from collections import defaultdict
from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class WeightedInstantRunoffMechanism(VotingMechanism):
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        total_weight_per_voter = {voter: sum(rank.weight for rank in ranking)
                                  for voter, ranking in rankings.items()}
        total_weight = sum(total_weight_per_voter.values())

        # Calculate the weight per proxy
        # Those without any first-place votes are automatically
        # eliminated since they would have no weight anyway
        votes = defaultdict(float)
        for voter, weight in total_weight_per_voter.items():
            proxy = rankings[voter].agent_ranked(1)
            votes[proxy] += weight

        max_vote_proxy = max(votes, key=votes.get)
        while votes[max_vote_proxy] < total_weight / 2:
            min_vote_proxy = min(votes, key=votes.get)
            for voter, ranking in rankings.items():
                removed_rank = ranking.rank_for(min_vote_proxy)
                ranking.remove_ranking(removed_rank)
                # If this was the first choice,
                # push their weight to their next choice
                if removed_rank == 1:
                    new_choice_agent = ranking.agent_ranked(1)
                    votes[new_choice_agent] += total_weight_per_voter[voter]

            del votes[min_vote_proxy]
            max_vote_proxy = max(votes, key=votes.get)

        return (sum(proxy.last_estimate * weight
                    for proxy, weight in votes.items())
                / total_weight)
