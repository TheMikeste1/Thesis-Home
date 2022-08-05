import proxy_estimate_system as pes

non_async = pes.ProxySystem(
        [pes.Agent(pes.distribution_strategies.GaussianDistribution(), 1)
         for _ in range(10)],
        [pes.InactiveVoter(
                pes.Agent(pes.distribution_strategies.UniformDistribution(), 1),
                pes.weighting_mechanisms.EqualWeightMechanism()) for _ in
            range(100)],
        pes.voting_mechanisms.WeightlessAllAverageMechanism(),
)

estimate = non_async.estimate(0)
if abs(estimate) > 0.5:
    print("Error: estimate is too large")
print(estimate)
