import proxy_system as ps


non_async = ps.ProxySystem(
    [ps.Agent(ps.distribution_strategies.GaussianDistribution(), 1)
     for _ in range(10)],
    [ps.InactiveVoter(
        ps.Agent(ps.distribution_strategies.UniformDistribution(), 1),
        ps.weighting_mechanisms.EqualWeightMechanism()) for _ in range(100)],
    ps.voting_mechanisms.WeightlessAllAverageMechanism()
)

estimate = non_async.estimate(0)
if abs(estimate) > 0.5:
    print("Error: estimate is too large")
print(estimate)
