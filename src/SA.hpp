#pragma once
#include "operators.hpp"
#include "PfspInstance.hpp"
#include "stats.hpp"
#include <vector>

class SA {
public:
  SA(const PfspInstance& instance, double initial_temp = -1.0,
     double cooling_rate = 0.995, int iterations = 10000);

  // Estimates a good initial temperature from 50 random solutions (10% of avg)
  static double estimate_initial_temp(const PfspInstance& instance);

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
