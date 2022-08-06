import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.candidate import PluralityMechanism

from util import TestAgent


class TestPluralityMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = PluralityMechanism()

        num_proxies = 11

        proxies = [TestAgent(i) for i in range(num_proxies)]
        rankings = {
            InactiveVoter(TestAgent(i), None):
                Rankings([RankingItem(i, proxies[i])])
            for i in range(len(proxies))}

        out = mechanism.solve(proxies, [], rankings)
        self.assertEqual(out, proxies[-1].last_estimate)

        rankings = {
            InactiveVoter(TestAgent(i), None):
                Rankings([RankingItem(num_proxies - i, proxies[i])])
            for i in range(len(proxies))}

        out = mechanism.solve(proxies, [], rankings)
        self.assertEqual(out, proxies[0].last_estimate)
