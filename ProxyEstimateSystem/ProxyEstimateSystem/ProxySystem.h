#pragma once

#include <cmath>
#include <utility>

#include "InactiveVoter.h"
#include "Rankings.h"
#include "TruthEstimator.h"
#include "VotingMechanisms/VotingMechanism.h"

class ProxySystem : public TruthEstimator
{
private:
   static const unsigned int RANDOM_SEED_EXTENT = UINT_MAX;

   std::vector<TruthEstimator*> _proxies;
   std::vector<InactiveVoter*> _voters;
   VotingMechanism* _votingMechanism;

protected:
   static void
   _updateAgentsEstimates(const std::vector<TruthEstimator*>& agents, double truth);

   double _generateEstimate(double truth) override;

   std::map<InactiveVoter*, Rankings>* _generateWeights();

public:
   ProxySystem(std::vector<TruthEstimator*> proxies, std::vector<InactiveVoter*> voters,
               VotingMechanism* votingMechanism)
   {
      this->_proxies = std::move(proxies);
      this->_voters = std::move(voters);
      this->_votingMechanism = votingMechanism;
   }

   std::vector<TruthEstimator*> getProxies() const
   {
      return this->_proxies;
   }

   std::vector<InactiveVoter*> getVoters() const
   {
      return this->_voters;
   }

   VotingMechanism* getVotingMechanism() const
   {
      return this->_votingMechanism;
   }

   void setVotingMechanism(VotingMechanism* votingMechanism)
   {
      this->_votingMechanism = votingMechanism;
   }
};
