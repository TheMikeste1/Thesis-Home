from abc import ABC, abstractmethod

from proxy_system.TruthEstimator import TruthEstimator


class WeightingMechanism(ABC):
    @abstractmethod
    def applyWeights(self, agent: TruthEstimator, proxies: [TruthEstimator]) -> Rankings:
        pass
