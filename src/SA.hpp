#pragma once
#include "operators.hpp"
#include "PfspInstance.hpp"
#include "stats.hpp"
#include <vector>

class SA {
public:
  SA(const PfspInstance& instance, double initial_temp = 1000.0,
     double cooling_rate = 0.995, int iterations = 10000);

  Solution run();
  SummaryStats runMultiple(int n_runs = 10);
  const std::vector<std::vector<Solution>>& getGenerationHistory() const { return history; }

private:
  const PfspInstance& instance;
  double initial_temp;
  double cooling_rate;
  int iterations;
  std::vector<std::vector<Solution>> history;
};
