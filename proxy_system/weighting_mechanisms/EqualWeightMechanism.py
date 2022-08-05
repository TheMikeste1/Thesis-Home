from proxy_system import RankingItem, Rankings, TruthEstimator
from proxy_system.weighting_mechanisms.WeightingMechanism import \
    WeightingMechanism


class EqualWeightMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        return Rankings([RankingItem(rank, 1, proxy)
                         for rank, proxy in enumerate(proxies)])
