// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <iostream>
#include <set>

#include "Utilities.h"
#include "Rankings.h"
#include "DistributionStrategies\GaussianDistribution.h"


using namespace std;


void loadingThingy()
{
   const char chars[8] = {'|', '/', '-', '\\', '|', '/', '-', '\\',};
   const size_t sizeChars = std::size(chars);

   int i = 0;
   while (true)
   {
      cout << chars[i] << "\r";
      ++i %= sizeChars;
      this_thread::sleep_for(chrono::milliseconds(100));
   }
}


int main()
{
   GaussianDistribution strategy(1561651);
   double min = INFINITY;
   double max = -INFINITY;
   int countInRange = 0;
   const int count = 10000;
   for (int i = 0; i < count; ++i)
   {
      double value = strategy.getValue(0, 1);

      if (value < min)
      {
         min = value;
      }
      if (value > max)
      {
         max = value;
      }

      if (value >= 0 && value <= 1)
      {
         ++countInRange;
      }

      cout << value << endl;
   }

   cout << min << " to " << max << endl;
   cout << countInRange << " in range (" << double(countInRange) / count * 100 << "%)" << endl;

   return 0;
}
