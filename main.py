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

MAX_PROXIES = 30
MIN_PROXIES = 1
PROXY_STEP = 1
PROXY_EXTENTS = {1}

MAX_INACTIVE = 30
MIN_INACTIVE = 1
INACTIVE_STEP = 1
INACTIVE_EXTENTS = {1}

NUM_ITERATIONS_PER_COMBO = 10

# random.seed(161803399)

WEIGHTING_MECHANISMS = {
    "Borda"      : pes.weighting_mechanisms.BordaMechanism,
    "Closest"    : pes.weighting_mechanisms.ClosestMechanism,
    "Distance"   : pes.weighting_mechanisms.DistanceMechanism,
    "EqualWeight": pes.weighting_mechanisms.EqualWeightMechanism,
    "NoOp"       : pes.weighting_mechanisms.NoOpMechanism,
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

WEIGHT_IGNORING_VMS = {
    "WeightlessAverageAll",
}


# %% Functions
def log(msg: str):
    print(f"[{datetime.datetime.now()}] {msg}")


def get_dataframe_from_files(dir_with_files: str,
                             verbose: bool = False) -> pd.DataFrame:
    if not dir_with_files.endswith('/'):
        dir_with_files += '/'

    files = os.listdir(dir_with_files)
    if verbose:
        print(f"Checking {len(files)} files. . .")
    data_files = []
    for i, file in enumerate(files, start=1):
        if verbose and i % 100 == 0:
            print(
                    f"Checking {i}/{len(files)} ("
                    f"{i / len(files) * 100:.2f}%). . .")
        path = f"{dir_with_files}{file}"
        if path.endswith(".feather") and os.path.isfile(path):
            data_files.append(path)

    if verbose:
        print(f"Reading {len(data_files)} files. . .")
    dfs = []
    for i, file in enumerate(data_files, start=1):
        if verbose and i % 100 == 0:
            print(
                    f"Reading {i}/{len(files)} ({i / len(files) * 100:.2f}%). "
                    f". .")
        dfs.append(pd.read_feather(file))
    print("Done reading! Concatenating. . .")
    return pd.concat(dfs)


def perform_iterations():
    proxy_counts = list(range(MIN_PROXIES, MAX_PROXIES + 1, PROXY_STEP))
    inactive_counts = list(range(MIN_INACTIVE, MAX_INACTIVE + 1, INACTIVE_STEP))
    combinations = product(proxy_counts, DISTRIBUTION_STRATEGIES, PROXY_EXTENTS,
                           inactive_counts, DISTRIBUTION_STRATEGIES,
                           INACTIVE_EXTENTS)
    paired_mechs = [
                       (v, "NoOp")
                       for v in WEIGHT_IGNORING_VMS
                   ] + [
                       (v, w)
                       for v in
                       set(VOTING_MECHANISMS) - set(WEIGHT_IGNORING_VMS)
                       for w in set(WEIGHTING_MECHANISMS) - {"NoOp"}
                   ]
    total_combos = len(proxy_counts) * len(DISTRIBUTION_STRATEGIES) \
                   * len(PROXY_EXTENTS) * len(inactive_counts) \
                   * len(DISTRIBUTION_STRATEGIES) * len(INACTIVE_EXTENTS) \
                   * len(paired_mechs)
    total_start = datetime.datetime.now()
    it_start = total_start
    log(f"Performing {total_combos:,} combinations")

    # Keeping track of rows as a Python list and then converting to a DataFrame
    # is much faster than just using a DataFrame for some reason.
    rows = []
    for voting_mechanism, inactive_weighting_mech in paired_mechs:
        for i, (num_proxies, proxy_dist_strategy, proxy_extent,
                num_inactive, inactive_dist_strategy, inactive_extent) \
                in enumerate(combinations, start=1):
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
                if not os.path.exists(f"{OUTPUT_DIR}/tmp"):
                    os.makedirs(f"{OUTPUT_DIR}/tmp")
                df = pd.DataFrame(rows)
                output_filename = f"PES" \
                                  f"_{it_start.strftime('%d-%m-%Y_%H-%M-%S')}" \
                                  f"_{len(df)}_rows"
                df.to_feather(f"./{OUTPUT_DIR}/tmp/{output_filename}.feather")
                rows = []

                current_time = datetime.datetime.now()
                log(f"{i:,}/{total_combos:,} ({i / total_combos * 100:.2f}%), "
                    f"{current_time - it_start} since last update, "
                    f"TOTAL: {current_time - total_start}")
                it_start = current_time

    log(f"Combining results. . .")
    results = get_dataframe_from_files(f"{OUTPUT_DIR}/tmp")
    output_filename = f"PES" \
                      f"_{len(results)}_rows" \
                      f"_{it_start.strftime('%d-%m-%Y_%H-%M-%S')}"
    results.to_feather(f"./{OUTPUT_DIR}/{output_filename}.feather")

    log(f"Cleaning up. . .")
    for file in os.listdir(f"./{OUTPUT_DIR}/tmp"):
        if file.startswith("PES") and \
                file.endswith(".feather") and \
                os.path.isfile(f"./{OUTPUT_DIR}/tmp/{file}"):
            os.remove(f"./{OUTPUT_DIR}/tmp/{file}")
    os.rmdir(f"./{OUTPUT_DIR}/tmp/")

    log(f"Completed in {datetime.datetime.now() - total_start}")


if __name__ == "__main__":
    # Run the experiment
    perform_iterations()
