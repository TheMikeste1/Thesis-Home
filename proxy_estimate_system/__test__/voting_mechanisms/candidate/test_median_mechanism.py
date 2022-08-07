import unittest
import random

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.candidate import MedianMechanism

from util import TestAgent


class TestMedianMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = MedianMechanism()

        num_proxies = 11

        # Simple middle
        proxies = [TestAgent(i) for i in range(num_proxies)]
        voters = [TestAgent(i) for i in range(num_proxies)]
        rankings = {
            InactiveVoter(voters[i], None):
                Rankings([RankingItem(1, proxies[i])])
            for i in range(len(proxies))}
        p = proxies.copy()
        random.shuffle(p)
        out = mechanism.solve(p, [], rankings)
        self.assertEqual(out, proxies[num_proxies // 2].last_estimate)

        # Large proxy not in middle
        rankings[voters[1]] = Rankings([RankingItem(99999, proxies[1])])
        p = proxies.copy()
        random.shuffle(p)
        out = mechanism.solve(p, [], rankings)
        self.assertEqual(out, proxies[1].last_estimate)
