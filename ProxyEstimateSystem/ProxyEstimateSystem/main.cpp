// ProxyEstimateSystem.cpp : Defines the entry point for the application.
//

#include <ctime>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>

#include "ArrowUtilities.h"
#include "Agent.h"
#include "DataRow.h"
#include "InactiveVoter.h"
#include "DistributionStrategies/DistributionStrategies.h"
#include "VotingMechanisms/VotingMechanisms.h"
#include "WeightingMechanisms/WeightingMechanisms.h"

void runSimulations(
      std::vector<DataRow>& generatedData,
      const int runsPerCombination,
      const int numberOfAgents,
      const std::map<std::string, std::function<DistributionStrategy*(
            void)>>& distributionStrategies,
      const std::map<std::string, std::function<VotingMechanism*(
            void)>>& votingMechanisms
)
{
   std::cout << "Starting simulations with\n";
   std::cout << "   " << numberOfAgents << " agents\n";
   std::cout << "   " << distributionStrategies.size() << " distribution strategies\n";
   std::cout << "   " << votingMechanisms.size() << " voting mechanisms\n";
   unsigned long long numberOfCombinations =
         distributionStrategies.size()
         * votingMechanisms.size()
         // -1 because we won't run a simulation with 0 inactive agents
         * (numberOfAgents - 1);
   std::cout << "   " << numberOfCombinations << " total combinations\n";
   std::cout << "   " << runsPerCombination << " runs per combination\n";

   generatedData.reserve(numberOfCombinations * runsPerCombination);

   for (int numInactiveAgents = 1;
        numInactiveAgents < numberOfAgents;
        numInactiveAgents++
         )
   {
      for (const auto& [distStrategyName, distributionStrategyFactory]: distributionStrategies)
      {
         // Set up the agents
         std::vector<Agent> agents;
         agents.reserve(numberOfAgents);
         for (int i = 0; i < numberOfAgents; i++)
         {
            agents.emplace_back(
                  distributionStrategyFactory(),
                  1
            );
         }
         // Set up the inactive agents
         std::vector<InactiveVoter*> inactiveAgents;
         inactiveAgents.reserve(numInactiveAgents);
         for (int i = 0; i < numInactiveAgents; i++)
         {
            auto* inactiveAgent = new InactiveVoter(
                  &agents[i],
                  new ClosestMechanism()
            );

            inactiveAgents.push_back(inactiveAgent);
         }

         // Get the list of proxies
         std::vector<TruthEstimator*> proxies;
         proxies.reserve(numberOfAgents - numInactiveAgents);
         for (int i = numInactiveAgents; i < numberOfAgents; i++)
         {
            proxies.push_back(&agents[i]);
         }

         assert(proxies.size() + inactiveAgents.size() == numberOfAgents);

         // Do the runs
         for (int i = 0; i < runsPerCombination; i++)
         {
            // Have each agent vote
            for (auto& agent: agents)
            {
               agent.estimate(0);
            }

            // Have each inactive agent weigh the proxies
            std::map<InactiveVoter*, Rankings> rankings;
            for (auto& inactiveAgent: inactiveAgents)
            {
               rankings[inactiveAgent] = inactiveAgent->weight(proxies);
            }

            // Apply each voting mechanism
            for (const auto& [vmName, votingMechanismFactory]: votingMechanisms)
            {
               auto vm = votingMechanismFactory();
               auto result = vm->solve(
                     proxies,
                     inactiveAgents,
                     rankings
               );

               std::stringstream ss;
               ss << distStrategyName << "_" << numInactiveAgents << "_" << i;

               auto id = ss.str();
               DataRow row = {
                     id,
                     distStrategyName,
                     vmName,
                     numberOfAgents - numInactiveAgents,
                     numInactiveAgents,
                     result
               };

               generatedData.push_back(row);
            }
         }

         // Cleanup inactive agents
         for (auto& inactiveAgent: inactiveAgents)
         {
            delete inactiveAgent;
         }
      }
   }
}

int main()
{
   const int runsPerCombo = 20;
   // Using the 117th Congress
   // https://history.house.gov/Institution/Party-Divisions/Party-Divisions/
   const int numberOfAgents = 435;
   std::map<std::string, std::function<DistributionStrategy*(void)>> distributions = {
         {"Uniform",    []() { return new UniformDistribution(); }},
         {"Gaussian",   []() { return new GaussianDistribution(); }},
         {"Beta_.3_.3", []() { return new BetaDistribution(0.3, 0.3); }},
         {"Beta_4_4",   []() { return new BetaDistribution(4, 4); }},
         {"Beta_4_1",   []() { return new BetaDistribution(4, 1); }},
         {"Beta_3_.3",  []() { return new BetaDistribution(3, 0.3); }},
   };
   std::map<std::string, std::function<VotingMechanism*(void)>> votingMechanisms = {
         // Average mechanisms
         {"Instant Runoff (Average)",            []() { return new average::InstantRunoffMechanism(); }},
         {"Mean",                                []() { return new average::MeanMechanism(); }},
         {"Ranked Choice",                       []() { return new average::RankedChoiceMechanism(); }},
         {"Weighted Instant Runoff (Average)",   []() { return new average::WeightedInstantRunoffMechanism(); }},
         {"Weightless Average All",              []() { return new average::WeightlessAverageAllMechanism(); }},

         // Candidate mechanisms
         {"Instant Runoff (Candidate)",          []() { return new candidate::InstantRunoffMechanism(); }},
         {"Median",                              []() { return new candidate::MedianMechanism(); }},
         {"Plurality",                           []() { return new candidate::PluralityMechanism(); }},
         {"Weighted Instant Runoff (Candidate)", []() { return new candidate::WeightedInstantRunoffMechanism(); }},
   };

   std::vector<DataRow> generatedData;
   runSimulations(generatedData, runsPerCombo, numberOfAgents, distributions,
                  votingMechanisms);

   auto t = std::time(nullptr);
   struct tm localTime = {};
   localtime_s(&localTime, &t);

   std::ostringstream oss;
   oss << std::put_time(&localTime, "%d-%m-%Y_%H-%M-%S") << ".feather";

   writeRowsToFile(oss.str(), generatedData);
   return 0;
}
