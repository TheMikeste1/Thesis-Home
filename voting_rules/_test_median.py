import unittest
from .median import median
from random import shuffle


class MedianTest(unittest.TestCase):
    def test_odd(self):
        # Sorted
        odd_list = [1, 2, 3, 4, 5]
        self.assertEqual(median(odd_list), 3)

        # Unsorted
        while odd_list == [1, 2, 3, 4, 5]:
            shuffle(odd_list)
        self.assertEqual(median(odd_list), 3)

    def test_even(self):
        # Sorted
        even_list = [1, 2, 3, 4]
        self.assertEqual(median(even_list), 2,
                         "Median should accept the lower value in a list "
                         "with even number of elements")

        # Unsorted
        while even_list == [1, 2, 3, 4]:
            shuffle(even_list)
        self.assertEqual(median(even_list), 2,
                         "Median should accept the lower value in a list "
                         "with even number of elements. Shuffled list: "
                         f"{even_list}")


if __name__ == '__main__':
    unittest.main()
