#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <cassert>
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

static const int BUDGET = 10000;  // eval budget per run
static const int EA_POP  = 100;
static const int EA_GEN  = 100;   // EA_POP * EA_GEN == BUDGET

int main() {
  static_assert(EA_POP * EA_GEN == BUDGET, "EA budget must equal BUDGET");

  for (const auto &path : TEST_CASES) {
    try {
      std::cout << "=== " << path << " ===\n";
      PfspInstance instance(path);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      instance.reset_eval_counter();
      std::vector<Solution> randomResult = instance.runRandomAlg(BUDGET);
      assert(instance.get_eval_counter() == BUDGET);
      Logger randomLogger(randomResult, base + "_randomAlg.csv");
      randomLogger.dumpToFile();
      std::cout << "[RANDOM] ";
      randomLogger.printBestTime();

      instance.reset_eval_counter();
      std::vector<Solution> greedyResult = instance.runGreedyAlg();
      std::cout << "[GREEDY] evals: " << instance.get_eval_counter() << " ";
      Logger greedyLogger(greedyResult, base + "_greedyAlg.csv");
      greedyLogger.dumpToFile();
      greedyLogger.printBestTime();

      instance.reset_eval_counter();
      EA ea(instance, EA_POP, EA_GEN);
      ea.run();
      assert(instance.get_eval_counter() == BUDGET);
      Logger eaLogger(ea.getGenerationHistory(), base + "_EA.csv");
      eaLogger.dumpToFile();
      std::cout << "[EA] ";
      eaLogger.printBestTime();

      instance.reset_eval_counter();
      SA sa(instance, 1000.0, 0.995, BUDGET);
      sa.run();
      assert(instance.get_eval_counter() == BUDGET);
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
