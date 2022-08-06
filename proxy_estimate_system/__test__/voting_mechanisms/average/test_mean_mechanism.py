import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.average import MeanMechanism

from util import TestAgent


class TestMeanMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = MeanMechanism()

        num_proxies = 10

        proxies = [TestAgent(i) for i in range(num_proxies)]
        rankings = {
            InactiveVoter(TestAgent(i), None):
                Rankings([RankingItem(1, proxies[i])])
            for i in range(len(proxies))}

        out = mechanism.solve(proxies, [], rankings)
        self.assertEqual(out, (num_proxies - 1) / 2)
