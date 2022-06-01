from typing import Any


def median(values: []):
    values = sorted(values)
    return values[(len(values) - 1) // 2]


def weighted_median(weighted_values: [(Any, float)]):
    weights = [w for v, w in weighted_values]

    midpoint = sum(weights) / 2

    weight_so_far = 0
    for (v, w) in weighted_values:
        weight_so_far += w
        if weight_so_far >= midpoint:
            return v
    raise ValueError("No median value found")
