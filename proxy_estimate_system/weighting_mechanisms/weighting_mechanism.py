from __future__ import annotations

from abc import ABC, abstractmethod
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from proxy_estimate_system import Rankings, TruthEstimator


class WeightingMechanism(ABC):
    @abstractmethod
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        pass
