from abc import ABCMeta, abstractmethod

import numpy as np

from proxy_system.seedable import Seedable


# I don't actually need ABCMeta since Seedable is an ABC, but I feel leaving
# it in helps to make it clear that this is an abstract class.
class DistributionStrategy(Seedable, metaclass=ABCMeta):
    def __init__(self, seed: int = None):
        if seed is not None:
            self._machine = np.random.default_rng(seed=seed)
        else:
            self._machine = np.random.default_rng()

    def set_seed(self, seed: int):
        self._machine = np.random.default_rng(seed=seed)

    @abstractmethod
    def get_value(self, minimum: float, maximum: float) -> float:
        pass
