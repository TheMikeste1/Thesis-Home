from proxy_estimate_system import TruthEstimator


class TestAgent(TruthEstimator):
    def __init__(self, estimate: float = None):
        self.last_estimate = estimate

    def _generate_estimate(self, truth: float) -> float:
        return self.last_estimate

    def set_seed(self, seed: int):
        pass

    def __str__(self):
        return f"TestAgent(last_estimate={self.last_estimate})"

    def __repr__(self):
        return self.__str__()

