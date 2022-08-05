import unittest

from proxy_estimate_system import TruthEstimator
from proxy_estimate_system.weighting_mechanisms import ClosestMechanism


class TestClosestMechanism(unittest.TestCase):
    def test_closest_mechanism(self):
        mechanism = ClosestMechanism()

        class TestAgent(TruthEstimator):
            def __init__(self, estimate: float = None):
                self.last_estimation = estimate

            def _generate_estimate(self, truth: float) -> float:
                pass

            def set_seed(self, seed: int):
                pass

            def __str__(self):
                return f"TestAgent(last_estimation={self.last_estimation})"

        agent = TestAgent(0)
        closest = TestAgent(0)
        others = [TestAgent(i) for i in range(1, 10)]

        out = mechanism.apply_weights(agent, [closest] + others)
        self.assertEqual(out.agent_ranked(1), closest)
