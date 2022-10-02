#pragma once
#include "DistributionStrategy.h"

class BetaDistribution : public DistributionStrategy
{
private:
   std::gamma_distribution<double> distributionX;
   std::gamma_distribution<double> distributionY;

public:
   BetaDistribution(double alpha, double beta)
   {
      distributionX = std::gamma_distribution<double>(alpha, 1);
      distributionY = std::gamma_distribution<double>(beta, 1);
   }

   BetaDistribution(double alpha, double beta, unsigned int seed) : BetaDistribution(alpha, beta)
   {
      this->seed(seed);
   }

   double getValue(double min, double max) override
   {
      // https://stackoverflow.com/a/10359049/10078500
      double x = distributionX(machine);
      double y = distributionY(machine);

      double z = x / (x + y);
      return z * (max - min) + min;  // Scale to [min, max]
   }
};
