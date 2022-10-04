#include "ProxySystem.h"

void ProxySystem::_updateAgentsEstimates(
      const std::vector<TruthEstimator*>& agents,
      double truth
)
{
   for (auto agent: agents)
   {
      agent->estimate(truth);
   }
}

double ProxySystem::_generateEstimate(double truth)
{
   _updateAgentsEstimates(this->_proxies, truth);

   std::vector<TruthEstimator*> vs;
   std::transform(
         this->_voters.begin(),
         this->_voters.end(),
         std::back_inserter(vs),
         [](InactiveVoter* voter) { return (TruthEstimator*) voter; }
   );
   _updateAgentsEstimates(vs, truth);

   auto weights = _generateWeights();

   auto estimate = this->_votingMechanism->solve(_proxies, _voters, *weights);

   delete weights;
   return estimate;
}

std::map<InactiveVoter*, Rankings>* ProxySystem::_generateWeights()
{
   auto* ret = new std::map<InactiveVoter*, Rankings>();
   for (auto voter: this->_voters)
   {
      (*ret)[voter] = voter->weight(_proxies);
   }
   return ret;
}
