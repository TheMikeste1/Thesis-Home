from __future__ import annotations

from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class MeanMechanism(VotingMechanism):
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_weights, system_weight = self._sum_proxy_weights(rankings)

        return (sum(proxy.last_estimate * weight
                    for proxy, weight in proxy_weights.items())
                / system_weight)
