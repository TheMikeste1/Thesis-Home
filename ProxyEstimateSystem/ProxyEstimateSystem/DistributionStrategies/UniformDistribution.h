#pragma once

#include <random>
#include "DistributionStrategy.h"

class UniformDistribution : public DistributionStrategy
{
private:
   std::uniform_real_distribution<double> distribution = std::uniform_real_distribution<double>(0, 1);

public:
   UniformDistribution() = default;
   UniformDistribution(unsigned int seed) : DistributionStrategy(seed) {}

   double getValue(double min, double max) override
   {
      return distribution(machine) * (max - min) + min;
   }
};
