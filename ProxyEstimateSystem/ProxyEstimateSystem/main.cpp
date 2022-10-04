// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <iostream>
#include <set>
#include <thread>

#include "Utilities.h"
#include "Rankings.h"
#include "DistributionStrategies\BetaDistribution.h"
#include "VotingMechanisms\Average\MeanMechanism.h"

using namespace std;


[[noreturn]] void loadingThingy()
{
   const char chars[8] = {'|', '/', '-', '\\', '|', '/', '-', '\\',};
   const size_t sizeChars = std::size(chars);

   int i = 0;
   while (true)
   {
      cout << chars[i] << "\r";
      ++i %= sizeChars;
      std::this_thread::sleep_for(chrono::milliseconds(100));
   }
}


int main()
{
   BetaDistribution strategy(0.3, 0.3);
   double min = INFINITY;
   double max = -INFINITY;
   int countInRange = 0;
   const int count = 100000;
   int p[100] = {};
   for (int i = 0; i < count; ++i)
   {
      double value = strategy.getValue(0, 100);

      if (value < min)
      {
         min = value;
      }
      if (value > max)
      {
         max = value;
      }

      if (value >= 0 && value < 100)
      {
         ++countInRange;
         p[(int) value]++;
      }

      // cout << value << endl;
   }

   cout << endl << "p: " << endl;
   for (int i = 0; i < 100; ++i)
   {
      std::cout << i << "-" << (i + 1) << ": ";
      std::cout << std::string(p[i] * count / count, '*') << std::endl;
   }

   cout << endl << min << " to " << max << endl;
   cout << countInRange << " in range (" << double(countInRange) / count * 100 << "%)"
        << endl;

   return 0;
}
