#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <iostream>

static const int BUDGET = 1000;

static const int    EA_POP  = 100;
static const int    EA_GEN  = 500;
static const float  EA_PX   = 0.7;
static const float  EA_PM   = 0.1;
static const int    EA_TOUR = 5;
static const CrossoverType EA_CROSS = CrossoverType::OX;
static const MutationType  EA_MUT   = MutationType::SWAP;
static const InitType      EA_INIT  = InitType::RANDOM;

static const double SA_TEMP    = -1.0;  // -1 = auto-estimate from instance
static const double SA_COOLING = 0.995;

static const std::vector<std::string> TEST_CASES = {
    "../test_cases/0.fsp",
    "../test_cases/tai20_5_0.fsp",
    "../test_cases/tai20_10_0.fsp",
    "../test_cases/tai20_20_0.fsp",
    "../test_cases/tai100_10_0.fsp",
    "../test_cases/tai100_20_0.fsp",
    "../test_cases/tai500_20_0.fsp",
};

int main() {
  for (const auto &path : TEST_CASES) {
    try {
      std::cout << "=== " << path << " ===\n";
      PfspInstance instance(path);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      std::vector<Solution> randomResult = instance.runRandomAlg(BUDGET);
      Logger randomLogger(randomResult, base + "_randomAlg.csv");
      randomLogger.dumpToFile();
      std::cout << "[RANDOM] ";
      randomLogger.printBestTime();

      std::vector<Solution> greedyResult = instance.runGreedyAlg();
      Logger greedyLogger(greedyResult, base + "_greedyAlg.csv");
      greedyLogger.dumpToFile();
      std::cout << "[GREEDY] ";
      greedyLogger.printBestTime();

      EA ea(instance, EA_POP, EA_GEN, EA_PX, EA_PM, EA_TOUR, EA_CROSS, EA_MUT, EA_INIT);
      ea.run();
      Logger eaLogger(ea.getGenerationHistory(), base + "_EA.csv");
      eaLogger.dumpToFile();
      std::cout << "[EA] ";
      eaLogger.printBestTime();

      SA sa(instance, SA_TEMP, SA_COOLING, BUDGET);
      sa.run();
      Logger saLogger(sa.getGenerationHistory(), base + "_SA.csv");
      saLogger.dumpToFile();
      std::cout << "[SA] ";
      saLogger.printBestTime();
    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
