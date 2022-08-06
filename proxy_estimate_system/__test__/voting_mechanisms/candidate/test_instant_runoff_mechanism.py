import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.candidate \
    import InstantRunoffMechanism

from util import TestAgent


class TestInstantRunoffMechanism(unittest.TestCase):
    def test_mechanism(self):
        mechanism = InstantRunoffMechanism()

        proxies = [TestAgent(0), TestAgent(1), TestAgent(2)]
        # Proxy 1 receives 2 votes, proxy 3 receives 1 vote.
        rankings = {
            InactiveVoter(TestAgent(0), None): Rankings(
                    [
                        RankingItem(2, proxies[0]),
                        RankingItem(1, proxies[1]),
                        RankingItem(0, proxies[2])
                    ]),
            InactiveVoter(TestAgent(1), None): Rankings(
                    [
                        RankingItem(2, proxies[0]),
                        RankingItem(1, proxies[1]),
                        RankingItem(0, proxies[2])
                    ]),
            InactiveVoter(TestAgent(2), None): Rankings(
                    [
                        RankingItem(0, proxies[0]),
                        RankingItem(1, proxies[1]),
                        RankingItem(2, proxies[2])
                    ])}

        out = mechanism.solve(proxies, rankings.keys(), rankings)
        self.assertEqual(out, proxies[0].last_estimate)
