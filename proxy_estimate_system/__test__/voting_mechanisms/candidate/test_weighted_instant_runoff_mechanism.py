import unittest

from proxy_estimate_system import InactiveVoter, RankingItem, Rankings
from proxy_estimate_system.voting_mechanisms.candidate \
    import WeightedInstantRunoffMechanism

from util import TestAgent


class TestWeightedInstantRunoffMechanism(unittest.TestCase):
    def test_mechanism(self):
        # Test based off
        # https://courses.lumenlearning.com/waymakermath4libarts/chapter/instant-runoff-voting/
        mechanism = WeightedInstantRunoffMechanism()

        proxies = {
            "A": TestAgent(0),
            "B": TestAgent(1),
            "C": TestAgent(2),
            "D": TestAgent(3),
            "E": TestAgent(4),
        }

        rankings = {
            # 3 votes for B C A D E
            InactiveVoter(TestAgent(0), None): Rankings([
                RankingItem(3, proxies["B"]),
                RankingItem(0, proxies["C"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["D"]),
                RankingItem(0, proxies["E"]),
            ]),

            # 4 votes for C A D B E
            InactiveVoter(TestAgent(1), None): Rankings([
                RankingItem(4, proxies["C"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["D"]),
                RankingItem(0, proxies["B"]),
                RankingItem(0, proxies["E"]),
            ]),

            # 4 votes for B D C A E
            InactiveVoter(TestAgent(2), None): Rankings([
                RankingItem(4, proxies["B"]),
                RankingItem(0, proxies["D"]),
                RankingItem(0, proxies["C"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["E"]),
            ]),

            # 6 votes for D C A E B
            InactiveVoter(TestAgent(3), None): Rankings([
                RankingItem(6, proxies["D"]),
                RankingItem(0, proxies["C"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["E"]),
                RankingItem(0, proxies["B"]),
            ]),

            # 2 votes for B E A C D
            InactiveVoter(TestAgent(4), None): Rankings([
                RankingItem(2, proxies["B"]),
                RankingItem(0, proxies["E"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["C"]),
                RankingItem(0, proxies["D"]),
            ]),

            # 1 vote for E A D B C
            InactiveVoter(TestAgent(5), None): Rankings([
                RankingItem(1, proxies["E"]),
                RankingItem(0, proxies["A"]),
                RankingItem(0, proxies["D"]),
                RankingItem(0, proxies["B"]),
                RankingItem(0, proxies["C"]),
            ]),
        }

        out = mechanism.solve(proxies.values(), rankings.keys(), rankings)
        self.assertEqual(out, proxies['D'].last_estimate)
