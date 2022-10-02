// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <iostream>
#include <set>

#include "Utilities.h"
#include "Rankings.h"


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
   Rankings rankings;
   rankings.insert(nullptr, 1.0);

   for (const auto& ranking : rankings)
   {
      cout << ranking.weight << endl;
   }

   return 0;
}
