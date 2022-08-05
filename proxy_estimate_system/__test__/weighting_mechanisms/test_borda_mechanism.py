import unittest

from proxy_estimate_system.weighting_mechanisms import BordaMechanism

from util import TestAgent


class TestBordaMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = BordaMechanism()

        agent_pos = 0
        num_agents = 10

        agent = TestAgent(agent_pos)
        proxies = [TestAgent(i)
                   for i in range(num_agents)]

        out = mechanism.apply_weights(agent, proxies)

        for rank, (proxy, item) in enumerate(zip(proxies, out)):
            self.assertEqual(item.proxy, proxy)
            self.assertEqual(item.weight, num_agents - rank)
