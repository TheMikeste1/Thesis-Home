import unittest

from proxy_estimate_system.distribution_strategies import GaussianDistribution


class TestGaussianDistribution(unittest.TestCase):
    def test_within_range(self):
        dist = GaussianDistribution()
        min_val = -3
        max_val = 3
        its = 100_000
        out_of_range = 0
        for _ in range(its):
            val = dist.get_value(min_val, max_val)
            if val < min_val or val > max_val:
                out_of_range += 1
        self.assertAlmostEqual(out_of_range / its * 100,  # Out of range
                                                          # percentage
                               (1 - 0.997) * 100,  # 99.7% of the time
                               delta=0.05,  # 0.05% margin of error
                               msg="GaussianDistribution should "
                                   "be within range 99.7% of the time")


if __name__ == '__main__':
    unittest.main()
