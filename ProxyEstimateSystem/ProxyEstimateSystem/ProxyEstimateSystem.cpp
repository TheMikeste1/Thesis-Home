// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <iostream>
#include "Rankings.h"

using namespace std;

int main()
{
   const char chars[8] = {'|', '/', '-', '\\', '|', '/', '-', '\\',};
   int sizeChars = std::size(chars);

   int i = 0;
   while (true)
   {
      cout << chars[i] << "\r";
      ++i %= sizeChars;
      this_thread::sleep_for(chrono::milliseconds(100));
   }

	return 0;
}
