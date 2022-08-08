from __future__ import annotations

from collections import Counter
from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class RankedChoiceMechanism(VotingMechanism):
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        weights = dict()

        # Start by removing proxies that have no votes
        votes = self.__count_votes(rankings)
        i = 0
        for i, proxy in enumerate(filter(lambda p: p not in votes, proxies),
                                  start=1):
            weights[proxy] = i
            self.__remove_agent_from_rankings(proxy, rankings.values())

        # Now calculate the ranking for the remaining proxies
        i += 1  # +1 to account for the removed proxies
        for i in range(i, len(proxies) + 1):
            votes = self.__count_votes(rankings)
            min_vote_proxy = min(votes, key=votes.get)
            weights[min_vote_proxy] = i
            self.__remove_agent_from_rankings(min_vote_proxy, rankings.values())

        return (sum(proxy.last_estimate * weight
                    for proxy, weight in weights.items())
                / sum(range(1, len(proxies) + 1)))

    @staticmethod
    def __count_votes(rankings) -> dict[TruthEstimator, int]:
        votes = Counter([ranking.agent_ranked(1)
                         for ranking in rankings.values()])
        return votes

    @staticmethod
    def __remove_agent_from_rankings(agent: TruthEstimator,
                                     rankings: [Rankings]):
        for ranking in rankings:
            ranking.remove_agent(agent)
