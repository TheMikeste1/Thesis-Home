/*
from .rankings import Rankings
from .truth_estimator import TruthEstimator
from .weighting_mechanisms.weighting_mechanism import WeightingMechanism


class InactiveVoter(TruthEstimator):
    def __init__(self, estimator: TruthEstimator,
                 weighting_mechanism: WeightingMechanism):
        self.__estimator = estimator
        self.__weighting_mechanism = weighting_mechanism

    @property
    def last_estimate(self) -> float:
        return self.__estimator.last_estimate

    @last_estimate.setter
    def last_estimate(self, value: float):
        self.__estimator.last_estimate = value

    @property
    def weighting_mechanism(self) -> WeightingMechanism:
        return self.__weighting_mechanism

    @weighting_mechanism.setter
    def weighting_mechanism(self, value: WeightingMechanism):
        self.__weighting_mechanism = value

    @property
    def estimator(self) -> TruthEstimator:
        return self.__estimator

    def weight(self, proxies: [TruthEstimator]) -> Rankings:
        if self.__estimator.last_estimate is None:
            raise ValueError('Cannot weight proxies without first making an '
                             'estimation.')
        return self.__weighting_mechanism.apply_weights(self, proxies)

    def _generate_estimate(self, truth: float) -> float:
        return self.__estimator.estimate(truth)

    def set_seed(self, seed: int):
        self.__estimator.set_seed(seed)

    def __str__(self):
        return f'InactiveVoter(estimator={self.__estimator}, ' \
               f'weighting_mechanism={self.__weighting_mechanism}) '
*/
class InactiveVoter
{
   
};
