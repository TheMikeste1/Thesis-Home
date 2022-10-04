#pragma once

#include <random>

class DistributionStrategy
{
protected:
   std::default_random_engine machine = std::default_random_engine(); // NOLINT(cert-msc51-cpp)
   
public:
   DistributionStrategy()
   {
      this->seed(time(nullptr));
   }

   explicit DistributionStrategy(unsigned int seed)
   {
      this->seed(seed);
   }
   virtual ~DistributionStrategy() = default;


   virtual double getValue(double min, double max) = 0;

   void seed(unsigned int seed)
   {
      machine.seed(seed);
   }
};
