import unittest

from proxy_estimate_system.weighting_mechanisms import DistanceMechanism

from util import TestAgent


class TestDistanceMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = DistanceMechanism()

        agent_pos = 0
        closest_pos = 0
        furthest_pos = 10

        agent = TestAgent(agent_pos)
        closest = TestAgent(closest_pos)
        others = [TestAgent(i) for i in
                  range(closest_pos + 1, furthest_pos + 1)]

        out = mechanism.apply_weights(agent, [closest] + others)
        self.assertEqual(out.agent_ranked(1), closest)
        self.assertEqual(out.weight_for_ranked(1), furthest_pos - closest_pos)
        self.assertEqual(
                out.weight_for_ranked(
                        furthest_pos - closest_pos + 1),  # Last ranked
                0)
