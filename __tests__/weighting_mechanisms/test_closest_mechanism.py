import unittest

from proxy_estimate_system.weighting_mechanisms import ClosestMechanism

from ..util.test_agent import TestAgent


class TestClosestMechanism(unittest.TestCase):
    def test_closest_mechanism(self):
        mechanism = ClosestMechanism()

        agent = TestAgent(0)
        closest = TestAgent(0)
        others = [TestAgent(i) for i in range(1, 10)]

        out = mechanism.apply_weights(agent, [closest] + others)
        self.assertEqual(out.agent_ranked(1), closest)
