from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism

from ..rankings import Rankings

if TYPE_CHECKING:
    from ..truth_estimator import TruthEstimator


class ClosestMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        closest = min(proxies, key=lambda p:
                      abs(p.last_estimation - agent.last_estimation))
        ret = Rankings()
        ret.add_ranking(1, 1, closest)
        for r, proxy in enumerate(filter(lambda p: p != closest, proxies),
                                  start=2):
            ret.add_ranking(r, 0, proxy)
        return ret
