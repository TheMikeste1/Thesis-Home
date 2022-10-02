/*
from __future__ import annotations

from abc import ABC, abstractmethod
from collections import defaultdict
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from proxy_estimate_system import InactiveVoter, Rankings, TruthEstimator


class VotingMechanism(ABC):
    @abstractmethod
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        pass

    @staticmethod
    def _sum_proxy_weights(rankings: dict[InactiveVoter, Rankings]) \
            -> (float, float):
        proxy_weights = defaultdict(float)

        system_weight = 0
        for ranking in rankings.values():
            for rank in ranking:
                system_weight += rank.weight
                proxy_weights[rank.proxy] += rank.weight

        return proxy_weights, system_weight
*/
