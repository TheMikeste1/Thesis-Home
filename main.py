import os
from functools import partial
from itertools import product
import random
import datetime

import pandas as pd

import proxy_estimate_system as pes

# %% Controls
OUTPUT_DIR = "data"
OUTPUT_INTERVAL = 100

MAX_PROXIES = 100
MIN_PROXIES = 10
PROXY_STEP = 10
PROXY_EXTENTS = {1}

MAX_INACTIVE = 100
MIN_INACTIVE = 10
INACTIVE_STEP = 10
INACTIVE_EXTENTS = {1}

NUM_ITERATIONS_PER_COMBO = 100

random.seed(161803399)

WEIGHTING_MECHANISMS = {
    "Borda"      : pes.weighting_mechanisms.BordaMechanism,
    "Closest"    : pes.weighting_mechanisms.ClosestMechanism,
    "Distance"   : pes.weighting_mechanisms.DistanceMechanism,
    "EqualWeight": pes.weighting_mechanisms.EqualWeightMechanism,
}

DISTRIBUTION_STRATEGIES = {
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

VOTING_MECHANISMS = {
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


# %% Functions
def perform_iterations():
    proxy_counts = list(range(MIN_PROXIES, MAX_PROXIES + 1, PROXY_STEP))
    inactive_counts = list(range(MIN_INACTIVE, MAX_INACTIVE + 1, INACTIVE_STEP))

    if not os.path.exists(f"{OUTPUT_DIR}/data"):
        os.makedirs(f"{OUTPUT_DIR}/data")

    combinations = product(proxy_counts, DISTRIBUTION_STRATEGIES, PROXY_EXTENTS,
                           inactive_counts, DISTRIBUTION_STRATEGIES,
                           INACTIVE_EXTENTS, WEIGHTING_MECHANISMS,
                           VOTING_MECHANISMS)

    total_combos = len(proxy_counts) * len(DISTRIBUTION_STRATEGIES) \
                   * len(PROXY_EXTENTS) * len(inactive_counts) \
                   * len(DISTRIBUTION_STRATEGIES) * len(INACTIVE_EXTENTS) \
                   * len(WEIGHTING_MECHANISMS) * len(VOTING_MECHANISMS)
    print(f"Performing {total_combos:,} combinations")

    total_start = datetime.datetime.now()
    it_start = total_start

    # Keeping track of rows as a Python list and then converting to a DataFrame
    # is much faster than just using a DataFrame for some reason.
    rows = []
    for i, (num_proxies, proxy_dist_strategy, proxy_extent,
            num_inactive, inactive_dist_strategy, inactive_extent,
            inactive_weighting_mech,
            voting_mechanism) in enumerate(combinations, start=1):
        # Create system
        proxies = [pes.Agent(
                DISTRIBUTION_STRATEGIES[proxy_dist_strategy](),
                proxy_extent)
            for _ in range(num_proxies)]
        inactive_voters = [pes.Agent(
                DISTRIBUTION_STRATEGIES[inactive_dist_strategy](),
                inactive_extent)
            for _ in range(num_inactive)]
        inactive_voters = [pes.InactiveVoter(
                a,
                WEIGHTING_MECHANISMS[inactive_weighting_mech]())
            for a in inactive_voters]
        vm = VOTING_MECHANISMS[voting_mechanism]()
        system = pes.ProxySystem(proxies, inactive_voters, vm)

        # Perform tests
        for _ in range(NUM_ITERATIONS_PER_COMBO):
            # run_seed = random.randint(0, 2 ** 64)
            # system.set_seed(run_seed)
            sout = system.estimate(0)
            rows.append({
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
                # "Seed"                      : run_seed,
            })

        # Print update
        if i % OUTPUT_INTERVAL == 0:
            # Output dataframe
            df = pd.DataFrame(rows)
            output_filename = f"PES_{len(df) * NUM_ITERATIONS_PER_COMBO}" \
                              f"_rows" \
                              f"_{it_start.strftime('%d-%m-%Y_%H-%M-%S')}"
            df.to_csv(f"./{OUTPUT_DIR}/data/{output_filename}.csv",
                      index=False)
            rows = []  # Reset the rows

            current_time = datetime.datetime.now()
            print(f"{i:,}/{total_combos:,} ({i / total_combos * 100:.2f}%) "
                  f"{current_time - it_start} since last update, "
                  f"TOTAL: {current_time - total_start}")
            it_start = current_time

    # Output if there is a remainder
    if rows:
        # Output dataframe
        df = pd.DataFrame(rows)
        output_filename = f"PES_{len(df) * NUM_ITERATIONS_PER_COMBO}" \
                          f"_rows" \
                          f"_{it_start.strftime('%d-%m-%Y_%H-%M-%S')}"
        df.to_csv(f"./{OUTPUT_DIR}/data/{output_filename}.csv", index=False)

    print(f"Completed in {datetime.datetime.now() - total_start:.2f}s")


if __name__ == "__main__":
    # Run the experiment
    perform_iterations()
