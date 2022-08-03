from proxy_system.distribution_strategies.DistributionStrategy \
    import DistributionStrategy


class GaussianDistribution(DistributionStrategy):
    def __init__(self, seed: int = None):
        super().__init__(seed)

    def get_value(self, minimum: float, maximum: float) -> float:
        val = self._machine.normal(0, scale=(maximum - minimum) / 3)
        return val + minimum
