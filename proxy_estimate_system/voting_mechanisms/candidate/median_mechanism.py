from __future__ import annotations

from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class MedianMechanism(VotingMechanism):
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_weights, system_weight = self._sum_proxy_weights(rankings)

        sum_ = 0
        for proxy in sorted(proxies, key=lambda x: proxy_weights[x]):
            sum_ += proxy_weights[proxy]
            if sum_ > system_weight / 2:
                return proxy.last_estimate
