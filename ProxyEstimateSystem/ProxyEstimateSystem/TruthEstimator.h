#pragma once

#include <stdexcept>

class TruthEstimator
{
private:
   double _lastEstimate = 0;
   bool _hasEstimate = false;

protected:
   virtual double _generateEstimate(double truth) = 0;

public:
   virtual ~TruthEstimator() = default;

   virtual double getLastEstimate() const
   {
      if (!_hasEstimate)
      {
         throw std::runtime_error("No estimate has been generated yet.");
      }
      return _lastEstimate;
   }

   double estimate(double truth)
   {
      this->_lastEstimate = _generateEstimate(truth);
      this->_hasEstimate = true;
      return _lastEstimate;
   }
};
