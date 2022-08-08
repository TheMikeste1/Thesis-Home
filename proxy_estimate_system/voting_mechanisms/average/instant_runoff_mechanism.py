from __future__ import annotations

from collections import Counter
from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class InstantRunoffMechanism(VotingMechanism):
    """
    Ignores weights. Uses instant runoff to eliminate proxies until some
    proxy has the majority, then averages using the votes as weights.
    """

    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        total_votes = len(inactive)
        votes = self.__count_votes(rankings)
        max_vote_proxy = max(votes, key=votes.get)

        while votes[max_vote_proxy] < total_votes / 2:
            min_vote_proxy = min(votes, key=votes.get)
            for ranking in rankings.values():
                ranking.remove_agent(min_vote_proxy)
            votes = self.__count_votes(rankings)
            max_vote_proxy = max(votes, key=votes.get)

        return (sum(proxy.last_estimate * weight
                    for proxy, weight in votes.items())
                / total_votes)

    @staticmethod
    def __count_votes(rankings) -> dict[TruthEstimator, int]:
        votes = Counter([ranking.agent_ranked(1)
                         for ranking in rankings.values()])
        return votes