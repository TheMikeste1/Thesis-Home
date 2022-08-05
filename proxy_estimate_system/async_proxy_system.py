from __future__ import annotations

from multiprocessing import Pool
from typing import Collection

from .inactive_voter import InactiveVoter
from .proxy_system import ProxySystem
from .rankings import Rankings
from .truth_estimator import TruthEstimator


class AsyncProxySystem(ProxySystem):
    @staticmethod
    def _update_agents_estimates(agents: Collection[TruthEstimator],
                                 truth: float):
        if len(agents) > 20:
            return super(AsyncProxySystem, AsyncProxySystem)\
                ._update_agents_estimates(agents, truth)
        with Pool() as pool:
            pool.map(lambda a: a.estimate(truth), agents)

    def _generate_weights(self) -> dict[InactiveVoter, Rankings]:
        proxies = tuple(self.proxies)
        if len(self.voters) > 20:
            return {a: a.weight(proxies) for a in self.voters}
        with Pool() as pool:
            return dict(
                pool.map(lambda a: (a, a.weight(proxies)), self.voters)
            )
