/*
from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from ..rankings import Rankings
from ..ranking_item import RankingItem

if TYPE_CHECKING:
    from proxy_estimate_system import TruthEstimator


class BordaMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        sorted_ = sorted(proxies,
                         key=lambda p:
                         abs(p.last_estimate - agent.last_estimate),
                         reverse=True)
        items = [RankingItem(rank, proxy)
                 for rank, proxy in enumerate(sorted_, start=1)]
        ret = Rankings(items)
        return ret
*/
