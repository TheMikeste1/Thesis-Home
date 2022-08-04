from abc import ABC, abstractmethod

from proxy_system.Rankings import Rankings
from proxy_system.TruthEstimator import TruthEstimator


class WeightingMechanism(ABC):
    @abstractmethod
    def apply_weights(self, agent: TruthEstimator, proxies: [TruthEstimator]) \
            -> Rankings:
        pass
