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
            void)>>& votingMechanisms,
      int iStart = 0
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
   std::cout << "   " << runsPerCombination * numberOfCombinations
             << " total runs\n";

   generatedData.reserve(numberOfCombinations * runsPerCombination);

   long long currentRow = 0;
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
      // Do the runs
      for (int i = 0; i < runsPerCombination; i++)
      {
         // Have each agent vote
         for (auto& agent: agents)
         {
            agent.estimate(0);
         }

         // Apply each voting mechanism
         for (const auto& [vmName, votingMechanismFactory]: votingMechanisms)
         {
            auto* vm = votingMechanismFactory();

            std::vector<TruthEstimator*> proxies;
            std::vector<InactiveVoter*> inactiveAgents;
            inactiveAgents.reserve(numberOfAgents - 1);
            proxies.reserve(numberOfAgents);
            for (auto& agent: agents)
            {
               proxies.push_back(&agent);
            }

            // Get the inactive agents, adding one additional agent each time.
            for (
                  int numInactiveAgents = 1;
                  numInactiveAgents < numberOfAgents;
                  numInactiveAgents++
                  )
            {
               // Set up the new inactive agent
               auto* newInactive = new InactiveVoter(
                     &agents[numberOfAgents - numInactiveAgents],
                     new ClosestMechanism()
               );
               inactiveAgents.push_back(newInactive);

               // Update the list of proxies by removing the now-inactive agent
               proxies.pop_back();

               assert(proxies.size() + inactiveAgents.size() == numberOfAgents);

               // Have each inactive agent weigh the proxies
               std::map<InactiveVoter*, Rankings> rankings;
               for (auto* inactiveAgent: inactiveAgents)
               {
                  rankings[inactiveAgent] = inactiveAgent->weight(proxies);
               }

               // Calculate min, max, and average weights
               // Sum the weights for each proxy
               std::map<TruthEstimator*, double> weights;
               for (auto* inactiveAgent: inactiveAgents)
               {
                  for (auto& [proxy, weight]: rankings[inactiveAgent])
                  {
                     weights[proxy] += weight;
                  }
               }
               assert(weights.size() == proxies.size());
               assert(weights.size() >= 0);
               // Find the min, max, and average weights
               double minWeight = 1;
               double maxWeight = 0;
               double totalWeight = 0;
               for (auto& [proxy, weight]: weights)
               {
                  minWeight = std::min(minWeight, weight);
                  maxWeight = std::max(maxWeight, weight);
                  totalWeight += weight;
               }
               double averageWeight = totalWeight / weights.size();

               auto result = vm->solve(
                     proxies,
                     inactiveAgents,
                     rankings
               );

               std::stringstream ss;
               ss << distStrategyName << "_" << numInactiveAgents << "_" << i + iStart;
               auto id = ss.str();

               DataRow row = {
                     id,
                     distStrategyName,
                     vmName,
                     numberOfAgents - numInactiveAgents,
                     numInactiveAgents,
                     result,
                     minWeight,
                     maxWeight,
                     averageWeight
               };

               generatedData.push_back(row);
               currentRow++;

               if (currentRow % 100 == 0)
               {
                  std::cout << std::fixed << std::setprecision(2)
                            << "\rRunning combination " << currentRow
                            << " ("
                            << static_cast<double>(currentRow)
                               / static_cast<double>(
                                     numberOfCombinations
                                     * runsPerCombination
                               ) * 100
                            << "%)";
               }
            }
            // Clean up the voting mechanism
            delete vm;
            // Clean up inactive agents
            for (auto& inactiveAgent: inactiveAgents)
            {
               delete inactiveAgent;
            }
         }
      }
   }
   std::cout << std::endl;
}


int main()
{
   auto filenameFormat = "%m-%d-%Y_%H-%M-%S";

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
         {"Weightless Average Proxies",          []() { return new average::WeightlessAverageProxiesMechanism(); }},

         // Candidate mechanisms
         {"Instant Runoff (Candidate)",          []() { return new candidate::InstantRunoffMechanism(); }},
         {"Median",                              []() { return new candidate::MedianMechanism(); }},
         {"Plurality",                           []() { return new candidate::PluralityMechanism(); }},
         {"Weighted Instant Runoff (Candidate)", []() { return new candidate::WeightedInstantRunoffMechanism(); }},
   };
   // Get start time
   auto t = std::time(nullptr);
   struct tm localTime = {};
   localtime_s(&localTime, &t);
   std::cout << "Starting at " << std::put_time(&localTime, "%H:%M:%S") << std::endl;
   std::vector<DataRow> generatedData;
   runSimulations(generatedData, runsPerCombo, numberOfAgents, distributions,
                  votingMechanisms, 20);

   // Write the data to a file
   std::cout << "Writing data to file...\n";
   std::ostringstream oss;
   oss << std::put_time(&localTime, filenameFormat) << ".feather";

   writeRowsToFile(oss.str(), generatedData);
   std::cout << "Done!\n";
   return 0;
}
