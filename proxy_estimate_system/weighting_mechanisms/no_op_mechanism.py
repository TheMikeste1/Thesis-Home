from typing import TYPE_CHECKING

from .weighting_mechanism import WeightingMechanism
from .. import Rankings

if TYPE_CHECKING:
    from proxy_estimate_system.truth_estimator import TruthEstimator


class NoOpMechanism(WeightingMechanism):
    def apply_weights(self, agent: TruthEstimator,
                      proxies: [TruthEstimator]) -> Rankings:
        return Rankings()
