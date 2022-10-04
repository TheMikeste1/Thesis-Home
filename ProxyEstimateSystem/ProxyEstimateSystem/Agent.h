#pragma once

#include "TruthEstimator.h"
#include "DistributionStrategies/DistributionStrategy.h"

class Agent : public TruthEstimator
{
private:
   DistributionStrategy* _distributionStrategy;
   double _extent;

protected:
   double _generateEstimate(double truth) override
   {
      return _distributionStrategy->getValue(truth - _extent, truth + _extent);
   }

public:
   Agent(DistributionStrategy* distributionStrategy, double extent)
         : _distributionStrategy(distributionStrategy), _extent(extent)
   {
   }

   DistributionStrategy* getDistributionStrategy() const
   {
      return _distributionStrategy;
   }

   void setDistributionStrategy(DistributionStrategy* distributionStrategy)
   {
      _distributionStrategy = distributionStrategy;
   }

   double getExtent() const
   {
      return _extent;
   }

   void setExtent(double extent)
   {
      _extent = extent;
   }

   void seed(unsigned int seed)
   {
      _distributionStrategy->seed(seed);
   }
};