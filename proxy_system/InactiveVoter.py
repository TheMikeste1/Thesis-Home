from proxy_system.Rankings import Rankings
from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.weighting_mechanisms.WeightingMechanism import \
    WeightingMechanism


class InactiveVoter(TruthEstimator):
    def __init__(self, estimator: TruthEstimator,
                 weightingMechanism: WeightingMechanism):
        self.__estimator = estimator
        self.__weightingMechanism = weightingMechanism

    @property
    def lastEstimation(self) -> float:
        return self.__estimator.lastEstimation

    @lastEstimation.setter
    def lastEstimation(self, value: float):
        self.__estimator.lastEstimation = value

    @property
    def weightingMechanism(self) -> WeightingMechanism:
        return self.__weightingMechanism

    @weightingMechanism.setter
    def weightingMechanism(self, value: WeightingMechanism):
        self.__weightingMechanism = value

    @property
    def estimator(self) -> TruthEstimator:
        return self.__estimator

    def weight(self, proxies: [TruthEstimator]) -> Rankings:
        if self.__estimator.lastEstimation is None:
            raise ValueError('Cannot weight proxies without first making an '
                             'estimation.')
        return self.__weightingMechanism.applyWeights(self, proxies)

    def _generate_estimate(self, truth: float) -> float:
        return self.__estimator.estimate(truth)

    def set_seed(self, seed: int):
        self.__estimator.set_seed(seed)
