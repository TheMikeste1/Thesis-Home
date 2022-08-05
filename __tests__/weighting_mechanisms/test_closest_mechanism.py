import unittest

from proxy_estimate_system.weighting_mechanisms import ClosestMechanism

from __tests__.util import TestAgent


class TestClosestMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = ClosestMechanism()

        agent = TestAgent(0)
        closest = TestAgent(0)
        others = [TestAgent(i) for i in range(1, 10)]

        out = mechanism.apply_weights(agent, [closest] + others)
        self.assertEqual(out.agent_ranked(1), closest)

        out.remove_ranking(1)
        prev = closest.last_estimation
        for item in out:
            self.assertEqual(item.weight, 0,
                             msg="All other items should have weight 0")
            self.assertGreater(item.proxy.last_estimation, prev,
                               msg="All items with the same weight should be "
                                   "ordered by insertion")
            prev = item.proxy.last_estimation
