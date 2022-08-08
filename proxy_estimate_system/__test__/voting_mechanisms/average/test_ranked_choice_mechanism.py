import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.average \
    import RankedChoiceMechanism

from util import TestAgent


class TestInstantRunoffMechanism(unittest.TestCase):
    def test_mechanism(self):
        # Test based off
        # https://courses.lumenlearning.com/waymakermath4libarts/chapter/instant-runoff-voting/
        mechanism = RankedChoiceMechanism()

        proxies = {
            "A": TestAgent(0),
            "B": TestAgent(1),
            "C": TestAgent(2),
            "D": TestAgent(3),
            "E": TestAgent(4),
        }

        rankings = {}
        # 3 votes for B C A D E
        for _ in range(3):
            voter = InactiveVoter(TestAgent(0), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["B"]),
                RankingItem(4, proxies["C"]),
                RankingItem(3, proxies["A"]),
                RankingItem(2, proxies["D"]),
                RankingItem(1, proxies["E"]),
            ])

        # 4 votes for C A D B E
        for _ in range(4):
            voter = InactiveVoter(TestAgent(1), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["C"]),
                RankingItem(4, proxies["A"]),
                RankingItem(3, proxies["D"]),
                RankingItem(2, proxies["B"]),
                RankingItem(1, proxies["E"]),
            ])

        # 4 votes for B D C A E
        for _ in range(4):
            voter = InactiveVoter(TestAgent(2), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["B"]),
                RankingItem(4, proxies["D"]),
                RankingItem(3, proxies["C"]),
                RankingItem(2, proxies["A"]),
                RankingItem(1, proxies["E"]),
            ])

        # 6 votes for D C A E B
        for _ in range(6):
            voter = InactiveVoter(TestAgent(3), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["D"]),
                RankingItem(4, proxies["C"]),
                RankingItem(3, proxies["A"]),
                RankingItem(2, proxies["E"]),
                RankingItem(1, proxies["B"]),
            ])

        # 2 votes for B E A C D
        for _ in range(2):
            voter = InactiveVoter(TestAgent(4), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["B"]),
                RankingItem(4, proxies["E"]),
                RankingItem(3, proxies["A"]),
                RankingItem(2, proxies["C"]),
                RankingItem(1, proxies["D"]),
            ])

        # 1 vote for E A D B C
        for _ in range(1):
            voter = InactiveVoter(TestAgent(5), None)
            rankings[voter] = Rankings([
                RankingItem(5, proxies["E"]),
                RankingItem(4, proxies["A"]),
                RankingItem(3, proxies["D"]),
                RankingItem(2, proxies["B"]),
                RankingItem(1, proxies["C"]),
            ])

        out = mechanism.solve(proxies.values(), rankings.keys(), rankings)
        expected = (proxies["A"].last_estimate * 1 +
                    proxies["E"].last_estimate * 2 +
                    proxies["C"].last_estimate * 3 +
                    proxies["B"].last_estimate * 4 +
                    proxies["D"].last_estimate * 5) / \
                   (1 + 2 + 3 + 4 + 5)
        self.assertEqual(out, expected)
