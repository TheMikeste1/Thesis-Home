def get_continuous_weights(values: [float], low: float, high: float) -> [tuple]:
    """
    Returns a list of value-weight pairs for the given values assuming the
    population is infinite, as per "Proxy Voting for Better Outcomes"
    chapter 2, paragraph 2.

    # Figure 1 second line of paper
    >>> v = [1, 3, 6, 7, ]
    >>> get_continuous_weights(v, 0, 10)
    [(1, 2.0), (3, 2.5), (6, 2.0), (7, 3.5)]

    # Figure 1 third line of paper
    >>> v = [1, 6, 7, ]
    >>> get_continuous_weights(v, 0, 10)
    [(1, 3.5), (6, 3.0), (7, 3.5)]
    """
    if len(values) == 0:
        return []

    low, high = min(low, high), max(low, high)

    values = sorted(values)
    assert low <= values[0] <= high, \
        "Values must be in the range [min_val, max_val]"
    assert low <= values[-1] <= high, \
        "Values must be in the range [min_val, max_val]"

    weights = [None] * len(values)

    # prev_val_border will keep track of where the last value's range ends
    prev_val_border = low
    # Loop through all but the last, calculating the weights
    for i in range(len(values) - 1):
        val = values[i]
        next_val = values[i + 1]
        weight = val - prev_val_border + (next_val - val) / 2
        weights[i] = weight
        # The new border will be halfway between the current value and the next
        prev_val_border = val + (next_val - val) / 2

    weights[-1] = high - prev_val_border
    return list(zip(values, weights))


def loss(predicted: float, real: float) -> float:
    """
    Equation 1 of the article.

    "E" in the equation just means the expected error, which is similar to
    the mean (https://math.stackexchange.com/a/1355657).

    For the paper, they were able to randomly select a subset of proxy voters
    from the population, then calculate the mean of those subsets. However,
    since my implementation will use specific proxies the loss only needs to be
    calculated once.

    >>> loss(1, 1)
    0.0
    >>> loss(1, 2)
    1.0
    >>> loss(2, 1)
    1.0
    """
    return (predicted - real) ** 2.0


if __name__ == "__main__":
    import doctest
    doctest.testmod()
