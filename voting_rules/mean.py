def mean(values: []):
    return sum(values) / len(values)


def weighted_mean(weighted_values: [(float, float)]):
    weights = [w for v, w in weighted_values]
    return sum([v * w for v, w in weighted_values]) / sum(weights)
