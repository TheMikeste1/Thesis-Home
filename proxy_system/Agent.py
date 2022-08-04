from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.distribution_strategies import DistributionStrategy


class Agent(TruthEstimator):
    def __init__(self, distributionStrategy: DistributionStrategy,
                 extent: float, ):
        self.__distributionStrategy = distributionStrategy
        self.__extent = extent

    @property
    def distributionStrategy(self) -> DistributionStrategy:
        return self.__distributionStrategy

    @distributionStrategy.setter
    def distributionStrategy(self, value: DistributionStrategy):
        self.__distributionStrategy = value

    @property
    def extent(self) -> float:
        return self.__extent

    @extent.setter
    def extent(self, value: float):
        self.__extent = value

    def _generateEstimate(self, truth: float) -> float:
        return self.__distributionStrategy.get_value(truth - self.__extent,
                                                     truth + self.__extent)
