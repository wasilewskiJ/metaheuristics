#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>

static const int N_RUNS = 5;
static const int BUDGET = 100000;

static const float  EA_PX   = 0.7;
static const float  EA_PM   = 0.1;
static const int    EA_TOUR = 5;
static const CrossoverType EA_CROSS = CrossoverType::OX;
static const MutationType  EA_MUT   = MutationType::SWAP;
static const InitType      EA_INIT  = InitType::RANDOM;

static const double SA_TEMP    = -1.0;  // -1 = auto-estimate from instance
static const double SA_COOLING = 0.995;

struct InstanceConfig {
  int   pop_size;
  float px;
};

static const std::vector<std::pair<std::string, InstanceConfig>> TEST_CASES = {
    {"../test_cases/tai20_5_0.fsp",   {5000, 0.90f}},
    {"../test_cases/tai20_10_0.fsp",  {5000, 0.90f}},
    {"../test_cases/tai20_20_0.fsp",  {5000, 0.70f}},
    {"../test_cases/tai100_10_0.fsp", {500,  0.90f}},
    {"../test_cases/tai100_20_0.fsp", {500,  0.95f}},
    {"../test_cases/tai500_20_0.fsp", {500,  0.98f}},
};

void print_stats(const std::string& label, const SummaryStats& s) {
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "  " << std::left << std::setw(20) << label
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

      int   pop = cfg.pop_size;
      float px  = cfg.px;
      int   gen = (BUDGET - pop) / pop;

      // --- Tune EA operator combinations ---
      std::cout << "-- EA operators --\n";
      for (auto [cx_label, cx] : std::vector<std::pair<std::string, CrossoverType>>{
          {"OX", CrossoverType::OX}, {"PMX", CrossoverType::PMX}, {"CX", CrossoverType::CX}}) {
        for (auto [mut_label, mut] : std::vector<std::pair<std::string, MutationType>>{
            {"SWAP", MutationType::SWAP}, {"INV", MutationType::INVERSION}}) {
          std::string label = cx_label + "+" + mut_label;
          instance.reset_eval_counter();
          EA ea(instance, pop, gen, px, EA_PM, EA_TOUR, cx, mut, EA_INIT);
          SummaryStats s = ea.runMultiple(N_RUNS);
          assert(instance.get_eval_counter() == N_RUNS * BUDGET);
          print_stats(label, s);
          Logger(ea.getBestRunHistory(), base + "_EA_" + label + ".csv").dumpToFile();
        }
      }

      // --- SA for reference ---
      std::cout << "-- SA --\n";
      SA sa(instance, SA_TEMP, SA_COOLING, BUDGET);
      instance.reset_eval_counter();
      SummaryStats s = sa.runMultiple(N_RUNS);
      assert(instance.get_eval_counter() == N_RUNS * (BUDGET + 1));
      print_stats("SA", s);
      Logger(sa.getBestRunHistory(), base + "_SA.csv").dumpToFile();

    } catch (const std::exception& e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }
  return 0;
}
