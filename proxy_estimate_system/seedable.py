from abc import ABC, abstractmethod


class Seedable(ABC):
    @abstractmethod
    def set_seed(self, seed: int):
        pass
