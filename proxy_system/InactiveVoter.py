from proxy_system.Rankings import Rankings
from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.weighting_mechanisms.WeightingMechanism import \
    WeightingMechanism


class InactiveVoter(TruthEstimator):
    def __init__(self, estimator: TruthEstimator,
                 weighting_mechanism: WeightingMechanism):
        self.__estimator = estimator
        self.__weighting_mechanism = weighting_mechanism

    @property
    def last_estimation(self) -> float:
        return self.__estimator.last_estimation

    @last_estimation.setter
    def last_estimation(self, value: float):
        self.__estimator.last_estimation = value

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
        if self.__estimator.last_estimation is None:
            raise ValueError('Cannot weight proxies without first making an '
                             'estimation.')
        return self.__weighting_mechanism.apply_weights(self, proxies)

    def _generate_estimate(self, truth: float) -> float:
        return self.__estimator.estimate(truth)

    def set_seed(self, seed: int):
        self.__estimator.set_seed(seed)
