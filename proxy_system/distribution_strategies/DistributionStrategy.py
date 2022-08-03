from abc import ABC, abstractmethod

import numpy as np


class DistributionStrategy(ABC):
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
