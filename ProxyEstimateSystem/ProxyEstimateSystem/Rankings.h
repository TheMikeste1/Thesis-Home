#pragma once

#include <vector>

#include "TruthEstimator.h"

using namespace std;

class Rankings
{
private:
   struct RankingsItem
   {
      double weight;
      TruthEstimator* proxy;
   };
   
   vector<RankingsItem> _rankings = vector<RankingsItem>();
   
public:
   Rankings() = default;
};

