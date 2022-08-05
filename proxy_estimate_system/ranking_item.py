from __future__ import annotations

from typing import TYPE_CHECKING
from dataclasses import dataclass

if TYPE_CHECKING:
    from .truth_estimator import TruthEstimator


@dataclass(frozen=True)
class RankingItem:
    rank: int
    weight: float
    proxy: TruthEstimator
