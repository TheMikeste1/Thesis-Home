from proxy_system import InactiveVoter, Rankings, TruthEstimator
from proxy_system.voting_mechanisms.VotingMechanism import VotingMechanism


class WeightlessAllAverageMechanism(VotingMechanism):

    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        proxy_estimates = [proxy.lastEstimate for proxy in proxies]
        inactive_estimates = [inactive.lastEstimate for inactive in inactive]
        return ((sum(proxy_estimates) + sum(inactive_estimates))
                / (len(proxy_estimates) + len(inactive_estimates)))
