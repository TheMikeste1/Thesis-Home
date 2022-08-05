from __future__ import annotations

from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from ..rankings import Rankings

if TYPE_CHECKING:
    from proxy_estimate_system import TruthEstimator


class BordaMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        raise NotImplementedError()

