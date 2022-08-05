from .distribution_strategy import DistributionStrategy


class GaussianDistribution(DistributionStrategy):
    def __init__(self, seed: int = None):
        super().__init__(seed)

    def get_value(self, minimum: float, maximum: float) -> float:
        half_range = (maximum - minimum) / 2
        val = self._machine.normal(minimum + half_range,
                                   scale=half_range / 3)
        return val
