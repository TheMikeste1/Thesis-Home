import unittest
from voting_rules.mean import weighted_mean


class WeightedMeanTestCase(unittest.TestCase):
    def test_paper_1(self):
        # Figure 1 second line of paper
        wv = [
            (1, 2),
            (3, 2.5),
            (6, 2),
            (7, 3.5),
        ]
        out = weighted_mean(wv)
        self.assertEqual(out, 4.6)

    def test_paper_2(self):
        # Figure 1 third line of paper
        wv = [
            (1, 3.5),
            (6, 3),
            (7, 3.5),
        ]
        out = weighted_mean(wv)
        self.assertEqual(out, 4.6)


if __name__ == '__main__':
    unittest.main()
