import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.candidate import MedianMechanism

from util import TestAgent


class TestMedianMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = MedianMechanism()

        num_proxies = 11

        proxies = [TestAgent(i) for i in range(num_proxies)]
        rankings = {
            InactiveVoter(TestAgent(i), None):
                Rankings([RankingItem(1, proxies[i])])
            for i in range(len(proxies))}

        out = mechanism.solve(proxies, [], rankings)
        self.assertEqual(out, proxies[num_proxies // 2].last_estimate)
