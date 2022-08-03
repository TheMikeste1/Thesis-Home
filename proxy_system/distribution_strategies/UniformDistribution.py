from .DistributionStrategy import DistributionStrategy


class UniformDistribution(DistributionStrategy):
    def __init__(self, seed: int = None):
        super().__init__(seed)

    def get_value(self, minimum: float, maximum: float) -> float:
        val = self._machine.uniform(minimum, maximum)
        return val
