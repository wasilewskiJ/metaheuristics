#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <iostream>

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
      std::vector<Solution> randomResult = instance.runRandomAlg(1000);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      Logger randomLogger(randomResult, base + "_randomAlg.csv");
      randomLogger.dumpToFile();
      std::cout << "[RANDOM] ";
      randomLogger.printBestTime();

      std::vector<Solution> greedyResult = instance.runGreedyAlg();
      Logger greedyLogger(greedyResult, base + "_greedyAlg.csv");
      greedyLogger.dumpToFile();
      std::cout << "[GREEDY] ";
      greedyLogger.printBestTime();

      EA ea(instance);
      ea.run();
      Logger eaLogger(ea.getGenerationHistory(), base + "_EA.csv");
      eaLogger.dumpToFile();
      std::cout << "[EA] ";
      eaLogger.printBestTime();

      SA sa(instance);
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
