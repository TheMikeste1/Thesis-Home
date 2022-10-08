#pragma once

#include <random>
#include "DistributionStrategy.h"

class GaussianDistribution : public DistributionStrategy
{
private:
   std::normal_distribution<double> distribution =
      std::normal_distribution(
         0.5, // Mean at 0.5 because it's the middle of [0, 1]
         0.5 / 3  // Std dev of 0.5 / 3 because we want 1 and 0 to be 3 std devs away
      );

public:
   GaussianDistribution() = default;
   explicit GaussianDistribution(unsigned int seed) : DistributionStrategy(seed) {}

   double getValue(double min, double max) override
   {
      return distribution(machine) * (max - min) + min;  // Scale to [min, max]
   }
};
