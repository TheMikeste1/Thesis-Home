#pragma once

#include <random>

class DistributionStrategy
{
protected:
   std::default_random_engine machine;
   
public:
   DistributionStrategy() : DistributionStrategy(std::random_device()()) {}

   explicit DistributionStrategy(unsigned int seed) // NOLINT(cert-msc51-cpp)
   {
      machine = std::default_random_engine(seed);
   }

   virtual ~DistributionStrategy() = default;


   virtual double getValue(double min, double max) = 0;

   void seed(unsigned int seed)
   {
      machine.seed(seed);
   }
};
