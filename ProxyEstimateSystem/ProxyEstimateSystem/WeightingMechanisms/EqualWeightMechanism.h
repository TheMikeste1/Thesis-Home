/*
from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from ..rankings import Rankings
from ..ranking_item import RankingItem

if TYPE_CHECKING:
    from proxy_estimate_system.truth_estimator import TruthEstimator


class EqualWeightMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        return Rankings([RankingItem(1, proxy)
                         for proxy in
                         sorted(proxies,
                                key=lambda p: abs(agent.last_estimate
                                                  - p.last_estimate))
                         ])
*/
