#include "Rankings.h"

#include <cassert>

#include "Utilities.h"


bool Rankings::_rankingInRange(int rank) const
{
   rank--;
   return rank >= 0 || rank < this->Size;
}

std::vector<Rankings::RankingsItem>::const_iterator
Rankings::_findAgent(TruthEstimator* proxy) const
{
   return std::ranges::find_if(
      _orderedRankings.begin(),
      _orderedRankings.end(),
      [proxy](const RankingsItem& item) { return item.proxy == proxy; });
}

TruthEstimator* Rankings::agentRanked(int ranking) const
{
   assert(this->_rankingInRange(ranking));
   return this->_orderedRankings[ranking - 1].proxy;
}

void Rankings::insert(TruthEstimator* proxy, double weight)
{
   this->removeAgent(proxy);

   binaryInsert<RankingsItem>(
      this->_orderedRankings,
      { proxy, weight },
      [](const RankingsItem& a, const RankingsItem& b) -> bool
      {
         return a.weight < b.weight;
      }
   );
}

int Rankings::rankFor(TruthEstimator* proxy) const
{
   const auto loc = this->_findAgent(proxy);
   return loc == this->_orderedRankings.end()
      ? -1
      : loc - this->_orderedRankings.begin() + 1;
}

void Rankings::removeAgent(TruthEstimator* proxy)
{
   const auto loc = this->_findAgent(proxy);
   if (loc != _orderedRankings.end())
   {
      _orderedRankings.erase(loc);
   }
}

void Rankings::removeRanking(int ranking)
{
   assert(this->_rankingInRange(ranking));
   _orderedRankings.erase(_orderedRankings.begin() + ranking - 1);
}
