import unittest

from proxy_estimate_system.distribution_strategies import BetaDistribution


class TestBetaDistribution(unittest.TestCase):
    def test_within_range(self):
        dist = BetaDistribution(a=1, b=1)
        min_val = 0
        max_val = 1
        for _ in range(1000):
            val = dist.get_value(min_val, max_val)
            self.assertGreaterEqual(val, min_val)
            self.assertLessEqual(val, max_val)


if __name__ == '__main__':
    unittest.main()
