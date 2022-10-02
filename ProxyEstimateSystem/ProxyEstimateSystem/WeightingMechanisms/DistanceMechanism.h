/*
from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from ..rankings import Rankings
from ..ranking_item import RankingItem

if TYPE_CHECKING:
    from proxy_estimate_system import TruthEstimator


class DistanceMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        if len(proxies) == 1:
            return Rankings([RankingItem(1, proxies[0])])

        distances = {proxy: abs(proxy.last_estimate - agent.last_estimate)
                     for proxy in proxies}
        max_distance = max(distances.values())

        # Flip ordering of distances so that the closest proxy has the highest
        # weight
        weights = {proxy: max_distance - dist
                   for proxy, dist in distances.items()}
        ret = Rankings([RankingItem(weight, proxy)
                        for proxy, weight in weights.items()])
        return ret
*/
