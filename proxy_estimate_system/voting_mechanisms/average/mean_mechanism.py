from __future__ import annotations

from typing import TYPE_CHECKING
from collections import defaultdict

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class MeanMechanism(VotingMechanism):
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_weights = defaultdict(float)

        system_weight = 0
        for ranking in rankings.values():
            for rank in ranking:
                system_weight += rank.weight
                proxy_weights[rank.proxy] += rank.weight

        return (sum(proxy.last_estimate * weight
                    for proxy, weight in proxy_weights.items())
                / system_weight)
