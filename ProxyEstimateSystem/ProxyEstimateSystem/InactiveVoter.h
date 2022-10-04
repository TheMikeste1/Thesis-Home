#pragma once

#include "TruthEstimator.h"
#include "WeightingMechanisms/WeightingMechanism.h"

class InactiveVoter : public TruthEstimator
{
private:
   TruthEstimator* _estimator;
   WeightingMechanism* _weightingMechanism;

protected:
   double _generateEstimate(double truth) override
   {
      return _estimator->estimate(truth);
   }

public:
   InactiveVoter(TruthEstimator* estimator, WeightingMechanism* weightingMechanism)
         : _estimator(estimator), _weightingMechanism(weightingMechanism)
   {
   }

   TruthEstimator* getEstimator() const
   {
      return _estimator;
   }

   void setEstimator(TruthEstimator* estimator)
   {
      _estimator = estimator;
   }

   WeightingMechanism* getWeightingMechanism() const
   {
      return _weightingMechanism;
   }

   void setWeightingMechanism(WeightingMechanism* weightingMechanism)
   {
      _weightingMechanism = weightingMechanism;
   }
};
