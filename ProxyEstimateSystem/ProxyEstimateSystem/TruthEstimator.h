#pragma once

class TruthEstimator
{
protected:
   double _lastEstimate;
   virtual double _generateEstimate(double truth) = 0;

public:
   double getLastEstimate() const
   {
      return _lastEstimate;
   }

   double estimate(double truth)
   {
      this->_lastEstimate = _generateEstimate(truth);
      return _lastEstimate;
   }
};
