// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <chrono>
#include <iostream>
#include <thread>

#include "Agent.h"
#include "ProxySystem.h"
#include "DistributionStrategies/DistributionStrategies.h"
#include "VotingMechanisms/Average/Average.h"
#include "VotingMechanisms/Candidate/Candidate.h"
#include "WeightingMechanisms/WeightingMechanisms.h"

#include "ArrowUtilities.h"

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
   // Test the Arrow utilities
   std::vector<DataRow> rows;
   rows.push_back({1.0});
   rows.push_back({2.0});
   rows.push_back({3.0});
   rows.push_back({4.0});
   rows.push_back({5.0});

   writeRowsToFile("test.feather", rows);

//   // Test for ProxySystem
//
//   // Create a bunch of proxies
//   std::vector<DistributionStrategy*> distStrategies;
//   std::vector<TruthEstimator*> proxies;
//   for (int i = 0; i < 100; ++i)
//   {
//      auto* strat = new BetaDistribution(0.3, 3);
//      distStrategies.push_back(strat);
//      auto* proxy = new Agent(strat, 1);
//      proxies.push_back(proxy);
//   }
//
//   // Create a bunch of voters
//   std::vector<InactiveVoter*> inactives;
//   std::vector<TruthEstimator*> agents(proxies);
//   std::vector<WeightingMechanism*> wms;
//   for (int i = 0; i < 100; ++i)
//   {
//      auto* strat = new GaussianDistribution();
//      distStrategies.push_back(strat);
//      auto* agent = new Agent(strat, 1);
//      agents.push_back(agent);
//      auto* wm = new ClosestMechanism();
//      wms.push_back(wm);
//      auto* voter = new InactiveVoter(agent, wm);
//      inactives.push_back(voter);
//   }
//
//   // Create a voting mechanism
//   auto* votingMechanism = new average::WeightlessAverageAllMechanism();
//
//   // Create a proxy system
//   ProxySystem pes(proxies, inactives, votingMechanism);
//
//   // Output a bunch of estimates
//   for (int i = 0; i < 100; ++i)
//   {
//      cout << pes.estimate(0) << endl;
//   }
//
//   // Clean up
//   for (auto* agent : agents)
//      delete agent;
//
//   for (auto* inactive : inactives)
//      delete inactive;
//
//   for (auto* strat : distStrategies)
//      delete strat;
//
//   for (auto* wm : wms)
//      delete wm;
//
//   delete votingMechanism;

   return 0;
}
