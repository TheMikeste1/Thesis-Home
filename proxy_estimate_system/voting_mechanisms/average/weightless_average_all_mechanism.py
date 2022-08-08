from __future__ import annotations

from typing import TYPE_CHECKING

from ..voting_mechanism import VotingMechanism

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class WeightlessAverageAllMechanism(VotingMechanism):
    """
    Ignores weights of proxies and inactive voters, calculates the average
    of all proxies and inactive voters.
    """
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_estimates = [proxy.last_estimate for proxy in proxies]
        inactive_estimates = [inactive.last_estimate for inactive in inactive]
        return ((sum(proxy_estimates) + sum(inactive_estimates))
                / (len(proxy_estimates) + len(inactive_estimates)))
