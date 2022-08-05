from abc import ABC, abstractmethod

from proxy_system.rankings import Rankings
from proxy_system.truth_estimator import TruthEstimator


class WeightingMechanism(ABC):
    @abstractmethod
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        pass
