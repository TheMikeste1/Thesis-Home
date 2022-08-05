from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from ..rankings import Rankings

if TYPE_CHECKING:
    from proxy_estimate_system import TruthEstimator


class DistanceMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        distances = {proxy: abs(proxy.estimate - agent.estimate)
                     for proxy in proxies}
        max_distance = max(distances.values())
        distances = {proxy: max_distance - dist
                     for proxy, dist in distances.items()}
        ret = Rankings()
        for r, proxy in enumerate(
                sorted(proxies, key=lambda p: distances[p]),
                start=1):
            weight = distances[proxy]
            ret.add_ranking(r, weight, proxy)
        return ret
