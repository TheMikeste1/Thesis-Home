from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism

from ..rankings import Rankings

if TYPE_CHECKING:
    from ..truth_estimator import TruthEstimator


class ClosestMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        sorted_ = sorted(proxies,
                         key=lambda p:
                         abs(p.last_estimate - agent.last_estimate))
        ret = Rankings()
        ret.add_ranking(1, sorted_[0])
        for proxy in sorted_[1:]:
            ret.add_ranking(0, proxy)
        return ret
