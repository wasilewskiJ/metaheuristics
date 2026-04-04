#include "PfspInstance.hpp"
#include "logger.hpp"

int main() {
  try {
    PfspInstance instance("0.fsp");
    std::vector<Solution> result = instance.runRandomAlg(100);
    Logger logger(result, "output.csv");
    logger.dumpToFile();
    logger.printBestTime();
  } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
      std::cerr << "Program exits.";
      return 1;
  }
  return 0;
}
