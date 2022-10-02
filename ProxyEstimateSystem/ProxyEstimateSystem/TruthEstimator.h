#pragma once

#include "Properties.h"

class TruthEstimator
{
protected:
   double _lastEstimate;
   virtual double _generateEstimate(double truth) = 0;

public:
   READONLY_PROPERTY(double, lastEstimate);
   GET(lastEstimate)
   {
      return _lastEstimate;
   }

   double estimate(double truth)
   {
      this->_lastEstimate = _generateEstimate(truth);
      return _lastEstimate;
   }
};
