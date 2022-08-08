from __future__ import annotations

from typing import TYPE_CHECKING, Iterable

from .ranking_item import RankingItem

if TYPE_CHECKING:
    from .truth_estimator import TruthEstimator


class Rankings:
    def __init__(self, weights: Iterable[RankingItem] = None):
        if weights is None:
            weights = []
        self.__rankings = sorted(weights, key=lambda rank: rank.weight,
                                 reverse=True)

    def rank_for(self, agent: TruthEstimator) -> int:
        for rank, item in enumerate(self.__rankings, start=1):
            if item.proxy == agent:
                return rank
        raise ValueError(f"Agent {agent} not in rankings")

    def weight_for(self, agent: TruthEstimator) -> float:
        return self.__rankings[self.rank_for(agent)].weight

    def agent_ranked(self, rank: int) -> TruthEstimator:
        self.__assert_rank_in_range(rank)
        return self.__rankings[rank - 1].proxy

    def weight_for_ranked(self, rank: int) -> float:
        self.__assert_rank_in_range(rank)
        return self.__rankings[rank - 1].weight

    def __iter__(self) -> Iterable[RankingItem]:
        return self.__rankings.__iter__()

    def add_ranking(self, weight: float, agent: TruthEstimator,
                    before_same_weight: bool = False):
        rank = 1
        for rank, item in enumerate(self.__rankings, start=2):
            if (before_same_weight and weight == item.weight) \
                    or weight > item.weight:
                break
        self.__rankings.insert(rank - 1, RankingItem(weight, agent))

    def remove_ranking(self, rank: int):
        self.__assert_rank_in_range(rank)
        del self.__rankings[rank - 1]

    def remove_agent(self, agent: TruthEstimator):
        for rank, item in enumerate(self.__rankings):
            if item.proxy == agent:
                del self.__rankings[rank]
                return
        raise ValueError(f"Agent {agent} not in rankings")

    def __assert_rank_in_range(self, rank: int):
        assert len(self.__rankings) > 0, "Empty rankings"
        assert 0 < rank <= len(self.__rankings), \
            f"Rank {rank} out of range, " \
            f"should be 1 <= rank <= {self.number_of_rankings} " \
            f"(the number of rankings)"

    @property
    def number_of_rankings(self) -> int:
        return len(self.__rankings)
