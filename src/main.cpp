#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>

static const int N_RUNS = 10;
static const int BUDGET = 200000;

static const int    EA_TOUR = 5;
static const CrossoverType EA_CROSS = CrossoverType::OX;
static const MutationType  EA_MUT   = MutationType::SWAP;
static const InitType      EA_INIT  = InitType::RANDOM;

struct InstanceConfig {
  int    pop_size;
  float  px;
  float  pm;
  double sa_cooling;
  double sa_t0_mult;
};

static const std::vector<std::pair<std::string, InstanceConfig>> TEST_CASES = {
    {"../test_cases/tai20_5_0.fsp",   {5000, 0.90f, 0.01f, 0.999, 10.0}},
    {"../test_cases/tai20_10_0.fsp",  {5000, 0.90f, 0.05f, 0.999, 10.0}},
    {"../test_cases/tai20_20_0.fsp",  {5000, 0.70f, 0.05f, 0.999,  1.0}},
    {"../test_cases/tai100_10_0.fsp", {500,  0.90f, 0.40f, 0.999,  1.0}},
    {"../test_cases/tai100_20_0.fsp", {500,  0.95f, 0.40f, 0.999,  5.0}},
    {"../test_cases/tai500_20_0.fsp", {500,  0.98f, 0.40f, 0.990,  0.5}},
};

void print_stats(const std::string& label, const SummaryStats& s) {
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "  " << std::left << std::setw(10) << label
            << " best=" << std::setw(8) << s.best
            << " avg="  << std::setw(10) << s.avg
            << " std="  << s.std_dev << "\n";
}

int main() {
  for (const auto& [path, cfg] : TEST_CASES) {
    try {
      std::cout << "\n=== " << path << " ===\n";
      PfspInstance instance(path);
      std::string base = path.substr(path.rfind('/') + 1);
      base = base.substr(0, base.rfind('.'));

      int    pop = cfg.pop_size;
      float  px  = cfg.px;
      float  pm  = cfg.pm;
      int    gen = (BUDGET - pop) / pop;
      double t0         = SA::estimate_initial_temp(instance) * cfg.sa_t0_mult;
      int    sa_log_int = std::max(1, BUDGET / 500);

      // --- Random (N_RUNS x BUDGET evals) ---
      {
        std::vector<int> best_per_run;
        for (int i = 0; i < N_RUNS; i++) {
          instance.reset_eval_counter();
          auto result = instance.runRandomAlg(BUDGET);
          assert(instance.get_eval_counter() == BUDGET);
          best_per_run.push_back(compute_stats(result).best);
          if (i == 0)
            Logger(result, base + "_randomAlg.csv").dumpToFile();
        }
        print_stats("RANDOM", compute_multirun_stats(best_per_run));
      }

      // --- Greedy (single multi-start run, budget = num_jobs) ---
      {
        instance.reset_eval_counter();
        auto result = instance.runGreedyAlg();
        SummaryStats s = compute_stats(result);
        Logger(result, base + "_greedyAlg.csv").dumpToFile();
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  " << std::left << std::setw(10) << "GREEDY"
                  << " best=" << std::setw(8) << s.best
                  << " evals=" << instance.get_eval_counter() << "\n";
      }

      // --- EA (N_RUNS x BUDGET evals) ---
      {
        instance.reset_eval_counter();
        EA ea(instance, pop, gen, px, pm, EA_TOUR, EA_CROSS, EA_MUT, EA_INIT);
        SummaryStats s = ea.runMultiple(N_RUNS);
        assert(instance.get_eval_counter() == N_RUNS * BUDGET);
        print_stats("EA", s);
        Logger(ea.getBestRunHistory(), base + "_EA.csv").dumpToFile();
      }

      // --- SA (N_RUNS x BUDGET evals) ---
      {
        SA sa(instance, t0, cfg.sa_cooling, BUDGET, sa_log_int);
        instance.reset_eval_counter();
        SummaryStats s = sa.runMultiple(N_RUNS);
        assert(instance.get_eval_counter() == N_RUNS * (BUDGET + 1));
        print_stats("SA", s);
        Logger(sa.getBestRunHistory(), base + "_SA.csv").dumpToFile();
      }

    } catch (const std::exception& e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
