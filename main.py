import os
from functools import partial
from itertools import product
import random
import time

import pandas as pd

import proxy_estimate_system as pes

if __name__ == "__main__":
    output_dir = "data"
    num_iterations_per_combo = 100

    random.seed(161803399)

    proxy_counts = set(range(5, 100 + 1, 5))
    proxy_extents = {1}

    inactive_counts = set(range(5, 100 + 1, 5))
    inactive_extents = {1}

    weighting_mechanisms = {
        "Borda"      : pes.weighting_mechanisms.BordaMechanism,
        "Closest"    : pes.weighting_mechanisms.ClosestMechanism,
        "Distance"   : pes.weighting_mechanisms.DistanceMechanism,
        "EqualWeight": pes.weighting_mechanisms.EqualWeightMechanism,
    }

    distribution_strategies = {
        "Uniform"   : pes.distribution_strategies.UniformDistribution,
        "Gaussian"  : pes.distribution_strategies.GaussianDistribution,
        "Beta_.3_.3": partial(pes.distribution_strategies.BetaDistribution,
                              a=0.3, b=0.3),
        "Beta_4_4"  : partial(pes.distribution_strategies.BetaDistribution,
                              a=4, b=4),
        "Beta_4_1"  : partial(pes.distribution_strategies.BetaDistribution,
                              a=4, b=1),
        "Beta_3_.3" : partial(pes.distribution_strategies.BetaDistribution,
                              a=3, b=0.3),
    }

    voting_mechanisms = {
        # Candidate
        "InstantRunoffCandidate"        :
            pes.voting_mechanisms.candidate.InstantRunoffMechanism,
        "Median"                        :
            pes.voting_mechanisms.candidate.MedianMechanism,
        "Plurality"                     :
            pes.voting_mechanisms.candidate.PluralityMechanism,
        "WeightedInstantRunoffCandidate":
            pes.voting_mechanisms.candidate.WeightedInstantRunoffMechanism,

        # Average
        "InstantRunoffAverage"          :
            pes.voting_mechanisms.average.InstantRunoffMechanism,
        "Mean"                          :
            pes.voting_mechanisms.average.MeanMechanism,
        "RankedChoice"                  :
            pes.voting_mechanisms.average.RankedChoiceMechanism,
        "WeightedInstantRunoffAverage"  :
            pes.voting_mechanisms.average.WeightedInstantRunoffMechanism,
        "WeightlessAverageAll"          :
            pes.voting_mechanisms.average.WeightlessAverageAllMechanism,
    }

    if not os.path.exists(f"{output_dir}/data"):
        os.makedirs(f"{output_dir}/data")

    combinations = product(proxy_counts, distribution_strategies, proxy_extents,
                           inactive_counts, distribution_strategies,
                           inactive_extents, weighting_mechanisms,
                           voting_mechanisms)

    total_combos = len(proxy_counts) * len(distribution_strategies) \
                   * len(proxy_extents) * len(inactive_counts) \
                   * len(distribution_strategies) * len(inactive_extents) \
                   * len(weighting_mechanisms) * len(voting_mechanisms)
    print(f"Performing {total_combos:,} combinations")

    total_start = time.time()
    it_start = total_start

    df = pd.DataFrame(columns=["ProxyCount", "ProxyDistribution", "ProxyExtent",
                               "InactiveCount", "InactiveDistribution",
                               "InactiveExtent",
                               "InactiveWeightingMechanism",
                               "VotingMechanism",
                               "SystemEstimate", "SquaredError",
                               "Seed"])
    for i, (num_proxies, proxy_dist_strategy, proxy_extent,
            num_inactive, inactive_dist_strategy, inactive_extent,
            inactive_weighting_mech,
            voting_mechanism) in enumerate(combinations, start=1):
        df = df.iloc[0:0]  # Reset the dataframe

        # Print update
        if i % 1_000 == 0:
            current_time = time.time()
            print(f"{i}/{total_combos} ({i / total_combos * 100:.2f}%) "
                  f"{current_time - it_start:.2f}s since last update, "
                  f"TOTAL: {current_time - total_start:.2f}s")
            it_start = current_time

        # Create system
        proxies = [pes.Agent(
                distribution_strategies[proxy_dist_strategy](),
                proxy_extent)
            for _ in range(num_proxies)]
        inactive_voters = [pes.Agent(
                distribution_strategies[inactive_dist_strategy](),
                inactive_extent)
            for _ in range(num_inactive)]
        inactive_voters = [pes.InactiveVoter(
                a,
                weighting_mechanisms[inactive_weighting_mech]())
            for a in inactive_voters]
        vm = voting_mechanisms[voting_mechanism]()
        system = pes.ProxySystem(proxies, inactive_voters, vm)

        # Perform tests
        for _ in range(num_iterations_per_combo):
            run_seed = random.randint(0, 2 ** 64)
            system.set_seed(run_seed)
            sout = system.estimate(0)
            df.loc[len(df)] = {
                "ProxyCount"                : num_proxies,
                "ProxyDistribution"         : proxy_dist_strategy,
                "ProxyExtent"               : proxy_extent,
                "InactiveCount"             : num_inactive,
                "InactiveDistribution"      : inactive_dist_strategy,
                "InactiveExtent"            : inactive_extent,
                "InactiveWeightingMechanism": inactive_weighting_mech,
                "VotingMechanism"           : voting_mechanism,
                "SystemEstimate"            : sout,
                "SquaredError"              : sout * sout,
                "Seed"                      : run_seed,
            }
        output_filename = f"{num_iterations_per_combo}_iterations-" \
                          f"{num_proxies}_proxies-" \
                          f"{proxy_dist_strategy}_proxy_dist-" \
                          f"{proxy_extent}_proxy_extent-" \
                          f"{num_inactive}_inactives-" \
                          f"{inactive_dist_strategy}_inactive_dist-" \
                          f"{inactive_extent}_inactive_extent-" \
                          f"{inactive_weighting_mech}_weighting-" \
                          f"{voting_mechanism}_voting.csv"
        df.to_csv(f"./{output_dir}/data/{output_filename}.csv", index=False)
    print(f"Completed in {time.time() - total_start:.2f}s")
