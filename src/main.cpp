#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>

static const int N_RUNS = 5;
static const int BUDGET = 100000;

static const std::vector<std::string> TEST_CASES = {
    "../test_cases/tai20_5_0.fsp",
    "../test_cases/tai20_10_0.fsp",
    "../test_cases/tai20_20_0.fsp",
    "../test_cases/tai100_10_0.fsp",
    "../test_cases/tai100_20_0.fsp",
    "../test_cases/tai500_20_0.fsp",
};

void print_stats(const std::string& label, const SummaryStats& s) {
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "  " << std::left << std::setw(28) << label
            << " best=" << std::setw(8) << s.best
            << " avg="  << std::setw(10) << s.avg
            << " std="  << s.std_dev << "\n";
}

int main() {
  for (const auto& path : TEST_CASES) {
    try {
      std::cout << "\n=== " << path << " ===\n";
      PfspInstance instance(path);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      // --- Tune SA: cooling_rate x T0 multiplier ---
      std::cout << "-- SA cooling x T0 --\n";
      double base_temp = SA::estimate_initial_temp(instance);
      for (double cooling : {0.990, 0.995, 0.999}) {
        for (double t0_mult : {0.5, 1.0, 5.0, 10.0}) {
          std::string label = "cr=" + std::to_string(cooling).substr(0,5)
                            + " t0x" + std::to_string(t0_mult).substr(0,4);
          SA sa(instance, base_temp * t0_mult, cooling, BUDGET);
          instance.reset_eval_counter();
          SummaryStats s = sa.runMultiple(N_RUNS);
          assert(instance.get_eval_counter() == N_RUNS * (BUDGET + 1));
          print_stats(label, s);
          Logger(sa.getBestRunHistory(), base + "_SA_" + label + ".csv").dumpToFile();
        }
      }

    } catch (const std::exception& e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
