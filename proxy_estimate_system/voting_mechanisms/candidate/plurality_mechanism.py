from __future__ import annotations

from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class PluralityMechanism(VotingMechanism):
    """
    Simply returns the estimate the proxy with the highest number of weight.
    """

    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_weights, _ = self._sum_proxy_weights(rankings)

        return max(proxy_weights, key=proxy_weights.get).last_estimate
