from abc import ABC, abstractmethod


class TruthEstimator():
    __last_estimate: float

    def estimate(self, truth: float) -> float:
        self.lastEstimation = self._generateEstimate(truth)
        return self.lastEstimation

    @abstractmethod
    def _generateEstimate(self, truth: float) -> float:
        pass

    @property
    def lastEstimation(self) -> float:
        # This is used to lazily initialize __last_estimate.
        # This allows subclasses to use __last_estimate without
        # forcing all subclasses to use it.
        # Subclasses that do not want to use it simply need to override
        # this property.
        if not hasattr(self, f'_{self.__class__.__name__}__last_estimate'):
            self.__last_estimate = None
        return self.__last_estimate

    @lastEstimation.setter
    def lastEstimation(self, value: float):
        self.__last_estimate = value

    def test(self):
        other = TruthEstimator()
        other.lastEstimation = 1.0
        assert self.lastEstimation != other.lastEstimation

