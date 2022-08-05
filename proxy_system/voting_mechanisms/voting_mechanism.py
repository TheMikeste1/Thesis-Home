from __future__ import annotations

from abc import ABC, abstractmethod
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from proxy_system import InactiveVoter
    from proxy_system import Rankings
    from proxy_system import TruthEstimator


class VotingMechanism(ABC):
    @abstractmethod
    def solve(self, proxies: [TruthEstimator], inactive: [InactiveVoter],
              rankings: dict[InactiveVoter, Rankings]) -> float:
        pass
