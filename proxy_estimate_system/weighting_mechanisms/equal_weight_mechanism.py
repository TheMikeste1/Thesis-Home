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
        return Rankings([RankingItem(rank, 1, proxy)
                         for rank, proxy in enumerate(proxies)])
