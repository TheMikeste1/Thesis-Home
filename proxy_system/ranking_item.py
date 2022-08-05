from dataclasses import dataclass

from proxy_system.TruthEstimator import TruthEstimator


@dataclass(frozen=True)
class RankingItem:
    rank: int
    weight: float
    proxy: TruthEstimator
