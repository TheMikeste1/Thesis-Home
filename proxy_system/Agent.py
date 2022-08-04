from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.distribution_strategies import DistributionStrategy


class Agent(TruthEstimator):
    def __init__(self, distribution_strategy: DistributionStrategy,
                 extent: float, ):
        self.__distribution_strategy = distribution_strategy
        self.__extent = extent

    @property
    def distribution_strategy(self) -> DistributionStrategy:
        return self.__distribution_strategy

    @distribution_strategy.setter
    def distribution_strategy(self, value: DistributionStrategy):
        self.__distribution_strategy = value

    @property
    def extent(self) -> float:
        return self.__extent

    @extent.setter
    def extent(self, value: float):
        self.__extent = value

    def set_seed(self, seed: int):
        self.__distribution_strategy.set_seed(seed)

    def _generate_estimate(self, truth: float) -> float:
        return self.__distribution_strategy.get_value(truth - self.__extent,
                                                      truth + self.__extent)
