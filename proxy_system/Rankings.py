from typing import Iterable

from proxy_system.RankingItem import RankingItem
from proxy_system.TruthEstimator import TruthEstimator


class Rankings:
    def __init__(self, rankings: Iterable[RankingItem] = None):
        if rankings is None:
            rankings = []
        self.__rankings = {rank.rank: rank for rank in rankings}

    def rankFor(self, agent: TruthEstimator) -> int:
        for rank in self.__rankings.values():
            if rank.proxy == agent:
                return rank.rank
        raise ValueError(f"Agent {agent} not in rankings")

    def weightFor(self, agent: TruthEstimator) -> float:
        return self.__rankings[self.rankFor(agent)].weight

    def agentRanked(self, rank: int) -> TruthEstimator:
        return self.__rankings[rank].proxy

    def weightForRanked(self, rank: int) -> float:
        return self.__rankings[rank].weight

    def __iter__(self) -> Iterable[RankingItem]:
        return sorted(self.__rankings.values(), key=lambda rank: rank.rank)

    def add_ranking(self, rank: int, weight: float, agent: TruthEstimator):
        self.__rankings[rank] = RankingItem(rank, weight, agent)

    def remove_ranking(self, rank: int):
        del self.__rankings[rank]
