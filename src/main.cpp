#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"
#include "stats.hpp"

#include <cassert>
#include <iomanip>
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

static const int BUDGET  = 10000;
static const int EA_POP  = 100;
static const int EA_GEN  = 100;    // EA_POP * EA_GEN == BUDGET
static const int N_RUNS  = 10;

static_assert(EA_POP * EA_GEN == BUDGET, "EA budget must equal BUDGET");

void print_stats(const std::string& label, const SummaryStats& s) {
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "[" << label << "] "
            << "best=" << s.best << " worst=" << s.worst
            << " avg=" << s.avg << " std=" << s.std_dev << "\n";
}

int main() {
  for (const auto &path : TEST_CASES) {
    try {
      std::cout << "=== " << path << " ===\n";
      PfspInstance instance(path);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      // Random - single run of BUDGET evaluations
      instance.reset_eval_counter();
      SummaryStats randStats = compute_stats(instance.runRandomAlg(BUDGET));
      assert(instance.get_eval_counter() == BUDGET);
      print_stats("RANDOM", randStats);

      // Greedy - J starting jobs
      instance.reset_eval_counter();
      SummaryStats greedyStats = compute_stats(instance.runGreedyAlg());
      std::cout << "[GREEDY] evals=" << instance.get_eval_counter() << " ";
      std::cout << "best=" << greedyStats.best << " worst=" << greedyStats.worst
                << " avg=" << greedyStats.avg << " std=" << greedyStats.std_dev << "\n";

      // EA - N_RUNS runs, log single best run for chart
      EA ea(instance, EA_POP, EA_GEN);
      SummaryStats eaStats = ea.runMultiple(N_RUNS);
      print_stats("EA", eaStats);
      // Single run for chart
      ea.run();
      Logger eaLogger(ea.getGenerationHistory(), base + "_EA.csv");
      eaLogger.dumpToFile();

      // SA - N_RUNS runs, same budget, log single best run for chart
      SA sa(instance, 1000.0, 0.995, BUDGET);
      SummaryStats saStats = sa.runMultiple(N_RUNS);
      print_stats("SA", saStats);
      // Single run for chart
      sa.run();
      Logger saLogger(sa.getGenerationHistory(), base + "_SA.csv");
      saLogger.dumpToFile();

    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
