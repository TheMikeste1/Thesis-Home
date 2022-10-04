#pragma once

#include <vector>

#include "TruthEstimator.h"

class Rankings
{
private:
   struct RankingsItem
   {
      TruthEstimator* proxy;
      double weight;
   };

   typedef std::vector<RankingsItem>::const_iterator iterator;

   std::vector<RankingsItem> _orderedRankings;

   bool _rankingInRange(int rank) const;
   std::vector<RankingsItem>::const_iterator _findAgent(TruthEstimator* proxy) const;

public:
   Rankings() = default;

   size_t size() const { return this->_orderedRankings.size(); }

   iterator begin() const { return _orderedRankings.cbegin(); }
   iterator end() const { return _orderedRankings.cend(); }

   TruthEstimator* agentRanked(int ranking) const;
   void insert(TruthEstimator* proxy, double weight);
   int rankFor(TruthEstimator* proxy) const;
   void removeAgent(TruthEstimator* proxy);
   void removeRanking(int ranking);
};
