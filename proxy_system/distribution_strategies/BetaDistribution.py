from proxy_system.distribution_strategies.DistributionStrategy \
    import DistributionStrategy


class BetaDistribution(DistributionStrategy):
    def __init__(self, a: float, b: float, seed: int = None):
        super().__init__(seed)
        self._a = a
        self._b = b

    def get_value(self, minimum: float, maximum: float) -> float:
        val = self._machine.beta(self._a, self._b)
        return val * (maximum - minimum) + minimum

    @property
    def a(self) -> float:
        return self._a

    @property
    def b(self) -> float:
        return self._b

    @a.setter
    def a(self, a: float):
        self._a = a

    @b.setter
    def b(self, b: float):
        self._b = b

    def __str__(self):
        return f"BetaDistribution(a={self._a}, b={self._b})"

    def __repr__(self):
        return self.__str__()
