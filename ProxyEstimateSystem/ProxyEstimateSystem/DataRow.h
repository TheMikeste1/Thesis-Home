#pragma once

#include <string>

struct DataRow
{
   std::string id;
   std::string distribution;
   std::string votingMechanism;
   int numberOfProxies;
   int numberOfInactives;
   double estimate;
   double minProxyWeight;
   double maxProxyWeight;
   double averageProxyWeight;
};
