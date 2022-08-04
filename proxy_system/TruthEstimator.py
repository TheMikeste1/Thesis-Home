from abc import ABCMeta, abstractmethod

from proxy_system.Seedable import Seedable


class TruthEstimator(Seedable, metaclass=ABCMeta):
    __last_estimate: float

    def estimate(self, truth: float) -> float:
        self.last_estimation = self._generate_estimate(truth)
        return self.last_estimation

    @abstractmethod
    def _generate_estimate(self, truth: float) -> float:
        pass

    @property
    def last_estimation(self) -> float:
        # This is used to lazily initialize __last_estimate.
        # This allows subclasses to use __last_estimate without
        # forcing all subclasses to use it.
        # Subclasses that do not want to use it simply need to override
        # this property.
        if not hasattr(self, f'_{self.__class__.__name__}__last_estimate'):
            self.__last_estimate = None
        return self.__last_estimate

    @last_estimation.setter
    def last_estimation(self, value: float):
        self.__last_estimate = value
