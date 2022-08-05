from proxy_system.rankings import Rankings
from proxy_system.ranking_item import RankingItem
from proxy_system.truth_estimator import TruthEstimator
from .WeightingMechanism import WeightingMechanism



class EqualWeightMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        return Rankings([RankingItem(rank, 1, proxy)
                         for rank, proxy in enumerate(proxies)])
