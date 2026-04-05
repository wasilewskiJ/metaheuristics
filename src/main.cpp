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
      std::vector<Solution> result = instance.runRandomAlg(1000);

      // derive output filename from instance name, e.g. "tai20_5_0.csv"
      std::string name = path.substr(path.rfind('/') + 1);
      name = name.substr(0, name.rfind('.')) + "_randomAlg.csv";

      Logger logger(result, name);
      logger.dumpToFile();
      logger.printBestTime();
    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
