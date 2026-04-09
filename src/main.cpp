#include "EA.hpp"
#include "SA.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"
#include "stats.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>

static const int BUDGET = 100000;
static const int N_RUNS_TUNING    = 5;
static const int N_RUNS_FINAL     = 10;

// Best config (set after tuning)
static const int    EA_POP  = 100;
static const int    EA_GEN  = 1000;  // EA_POP * EA_GEN == BUDGET
static const float  EA_PX   = 0.7;
static const float  EA_PM   = 0.1;
static const int    EA_TOUR = 5;
static const CrossoverType EA_CROSS = CrossoverType::OX;
static const MutationType  EA_MUT   = MutationType::SWAP;

static const double SA_TEMP    = -1.0;   // -1 = auto-estimate from instance
static const double SA_COOLING = 0.995;

static const std::vector<std::string> TUNING_EASY   = { "../test_cases/tai20_5_0.fsp" };
static const std::vector<std::string> TUNING_MEDIUM  = { "../test_cases/tai100_10_0.fsp" };
static const std::vector<std::string> TUNING_HARD    = { "../test_cases/tai500_20_0.fsp" };

static const std::vector<std::string> ALL_INSTANCES = {
    "../test_cases/tai20_5_0.fsp",
    "../test_cases/tai20_10_0.fsp",
    "../test_cases/tai20_20_0.fsp",
    "../test_cases/tai100_10_0.fsp",
    "../test_cases/tai100_20_0.fsp",
    "../test_cases/tai500_20_0.fsp",
};

void print_stats(const std::string& label, const SummaryStats& s) {
  std::cout << std::fixed << std::setprecision(1);
  std::cout << std::left << std::setw(30) << label
            << " best=" << std::setw(8) << s.best
            << " worst=" << std::setw(8) << s.worst
            << " avg=" << std::setw(10) << s.avg
            << " std=" << s.std_dev << "\n";
}

void run_tuning(const std::string& path) {
  PfspInstance instance(path);
  std::cout << "\n=== TUNING: " << path << " ===\n";

  // --- EA: varying pop_size (generations adjusted to keep budget) ---
  std::cout << "\n-- EA pop_size --\n";
  for (int pop : {50, 100, 200}) {
    int gen = BUDGET / pop;
    EA ea(instance, pop, gen, EA_PX, EA_PM, EA_TOUR, EA_CROSS, EA_MUT);
    print_stats("pop=" + std::to_string(pop) + " gen=" + std::to_string(gen),
                ea.runMultiple(N_RUNS_TUNING));
  }

  // --- EA: varying Px ---
  std::cout << "\n-- EA Px --\n";
  for (float px : {0.5f, 0.7f, 0.9f}) {
    EA ea(instance, EA_POP, EA_GEN, px, EA_PM, EA_TOUR, EA_CROSS, EA_MUT);
    print_stats("Px=" + std::to_string(px), ea.runMultiple(N_RUNS_TUNING));
  }

  // --- EA: varying Pm ---
  std::cout << "\n-- EA Pm --\n";
  for (float pm : {0.01f, 0.1f, 0.3f}) {
    EA ea(instance, EA_POP, EA_GEN, EA_PX, pm, EA_TOUR, EA_CROSS, EA_MUT);
    print_stats("Pm=" + std::to_string(pm), ea.runMultiple(N_RUNS_TUNING));
  }

  // --- EA: crossover types ---
  std::cout << "\n-- EA crossover --\n";
  for (auto [label, cx] : std::vector<std::pair<std::string, CrossoverType>>{
      {"OX", CrossoverType::OX}, {"PMX", CrossoverType::PMX}, {"CX", CrossoverType::CX}}) {
    EA ea(instance, EA_POP, EA_GEN, EA_PX, EA_PM, EA_TOUR, cx, EA_MUT);
    print_stats("crossover=" + label, ea.runMultiple(N_RUNS_TUNING));
  }

  // --- EA: mutation types ---
  std::cout << "\n-- EA mutation --\n";
  for (auto [label, mut] : std::vector<std::pair<std::string, MutationType>>{
      {"SWAP", MutationType::SWAP}, {"INVERSION", MutationType::INVERSION}}) {
    EA ea(instance, EA_POP, EA_GEN, EA_PX, EA_PM, EA_TOUR, EA_CROSS, mut);
    print_stats("mutation=" + label, ea.runMultiple(N_RUNS_TUNING));
  }

  // --- SA: varying initial_temp (scaled to instance) ---
  std::cout << "\n-- SA initial_temp --\n";
  double base_temp = SA::estimate_initial_temp(instance);
  for (double factor : {0.1, 1.0, 10.0}) {
    double temp = base_temp * factor;
    SA sa(instance, temp, SA_COOLING, BUDGET);
    print_stats("temp=" + std::to_string(temp), sa.runMultiple(N_RUNS_TUNING));
  }

  // --- SA: varying cooling_rate ---
  std::cout << "\n-- SA cooling_rate --\n";
  for (double cr : {0.99, 0.995, 0.999}) {
    SA sa(instance, SA_TEMP, cr, BUDGET);
    print_stats("cooling=" + std::to_string(cr), sa.runMultiple(N_RUNS_TUNING));
  }
}

void run_comparison(const std::string& path) {
  PfspInstance instance(path);
  std::cout << "\n=== COMPARISON: " << path << " ===\n";
  std::string base = path.substr(path.rfind('/') + 1);
  base = base.substr(0, base.rfind('.'));

  // Random
  instance.reset_eval_counter();
  SummaryStats randStats = compute_stats(instance.runRandomAlg(BUDGET));
  assert(instance.get_eval_counter() == BUDGET);
  print_stats("RANDOM", randStats);

  // Greedy
  instance.reset_eval_counter();
  SummaryStats greedyStats = compute_stats(instance.runGreedyAlg());
  std::cout << std::left << std::setw(30) << "GREEDY"
            << " evals=" << instance.get_eval_counter()
            << " best=" << greedyStats.best << " worst=" << greedyStats.worst
            << " avg=" << greedyStats.avg << " std=" << greedyStats.std_dev << "\n";

  // EA - 10 runs + single run for chart
  EA ea(instance, EA_POP, EA_GEN, EA_PX, EA_PM, EA_TOUR, EA_CROSS, EA_MUT);
  print_stats("EA", ea.runMultiple(N_RUNS_FINAL));
  ea.run();
  Logger(ea.getGenerationHistory(), base + "_EA.csv").dumpToFile();

  // SA - 10 runs + single run for chart
  SA sa(instance, SA_TEMP, SA_COOLING, BUDGET);
  print_stats("SA", sa.runMultiple(N_RUNS_FINAL));
  sa.run();
  Logger(sa.getGenerationHistory(), base + "_SA.csv").dumpToFile();
}

int main(int argc, char* argv[]) {
  bool do_tuning = (argc > 1 && std::string(argv[1]) == "--tune");

  if (do_tuning) {
    std::cout << "### PARAMETER TUNING ###\n";
    for (const auto& path : TUNING_EASY)   run_tuning(path);
    for (const auto& path : TUNING_MEDIUM) run_tuning(path);
    for (const auto& path : TUNING_HARD)   run_tuning(path);
  } else {
    std::cout << "### FINAL COMPARISON ###\n";
    for (const auto& path : ALL_INSTANCES)
      run_comparison(path);
  }

  return 0;
}
