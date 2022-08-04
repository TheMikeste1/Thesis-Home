from random import Random

from typing import Collection

from proxy_system.InactiveVoter import InactiveVoter
from proxy_system.Rankings import Rankings
from proxy_system.TruthEstimator import TruthEstimator
from proxy_system.voting_mechanisms.VotingMechanism import VotingMechanism


class ProxySystem(TruthEstimator):
    __random_seed_extent = 2 ** 64

    def __init__(self, proxies: Collection[TruthEstimator],
                 voters: Collection[InactiveVoter],
                 voting_mechanism: VotingMechanism,
                 seed: int = None):
        super().__init__(seed)
        self.__proxies = proxies
        self.__voters = voters
        self.__voting_mechanism = voting_mechanism
        if seed is not None:
            self.set_seed(seed)

    @property
    def proxies(self) -> Collection[TruthEstimator]:
        return self.__proxies

    @property
    def voters(self) -> Collection[InactiveVoter]:
        return self.__voters

    @property
    def voting_mechanism(self) -> VotingMechanism:
        return self.__voting_mechanism

    @voting_mechanism.setter
    def voting_mechanism(self, votingMechanism: VotingMechanism):
        self.__voting_mechanism = votingMechanism

    def _generate_estimate(self, truth: float) -> float:
        self._update_agents_estimates(self.voters, truth)
        self._update_agents_estimates(self.proxies, truth)
        weights = self._getWeights()
        return self.voting_mechanism.solve(self.proxies, self.voters, weights)

    def set_seed(self, seed: int):
        gen = Random(seed)
        for agent in list(self.proxies) + list(self.voters):
            agent.set_seed(gen.randint(-ProxySystem.__random_seed_extent,
                                       ProxySystem.__random_seed_extent))

    @staticmethod
    def _update_agents_estimates(agents: Collection[TruthEstimator],
                                 truth: float):
        for a in agents:
            a.estimate(truth)

    def _getWeights(self) -> dict[InactiveVoter, Rankings]:
        proxies = tuple(self.proxies)
        return {a: a.weight(proxies) for a in self.voters}
