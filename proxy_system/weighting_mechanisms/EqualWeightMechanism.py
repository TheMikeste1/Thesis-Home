from proxy_system.Rankings import Rankings
from proxy_system.RankingItem import RankingItem
from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.weighting_mechanisms.WeightingMechanism import \
    WeightingMechanism


class EqualWeightMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        return Rankings([RankingItem(rank, 1, proxy)
                         for rank, proxy in enumerate(proxies)])
