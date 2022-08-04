from abc import ABC, abstractmethod

from proxy_system.InactiveVoter import InactiveVoter
from proxy_system.Rankings import Rankings
from proxy_system.TruthEstimator import TruthEstimator


class VotingMechanism(ABC):
    @abstractmethod
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        pass
