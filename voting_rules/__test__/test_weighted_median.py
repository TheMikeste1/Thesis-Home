import unittest
from voting_rules.median import weighted_median


class WeightedMedianTestCase(unittest.TestCase):
    def test_paper_1(self):
        # Figure 1 second line of paper
        wv = [
            ("s1", 2),
            ("s2", 2.5),
            ("s3", 2),
            ("s4", 3.5),
        ]
        out = weighted_median(wv)
        self.assertEqual(out, "s3")

    def test_paper_2(self):
        # Figure 1 third line of paper
        wv = [
            ("s1", 3.5),
            ("s3", 3),
            ("s4", 3.5),
        ]
        out = weighted_median(wv)
        self.assertEqual(out, "s3")


if __name__ == '__main__':
    unittest.main()
