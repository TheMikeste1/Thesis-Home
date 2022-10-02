/*
from __future__ import annotations

from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class MedianMechanism(VotingMechanism):
    """
    Gives the weighted median proxy's estimate, proxy whose weight sums with
    those below it when sorted by estimate to is more the half the total
    system weight.

    When all proxies have the same weight, the median is simply the center
    proxy when sorted by estimate.
    """

    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_weights, system_weight = self._sum_proxy_weights(rankings)

        sum_ = 0
        for proxy in sorted(proxies, key=lambda x: x.last_estimate):
            sum_ += proxy_weights[proxy]
            if sum_ > system_weight / 2:
                return proxy.last_estimate
*/
