#pragma once
#include "operators.hpp"
#include "PfspInstance.hpp"
#include "logger.hpp"
#include <vector>

class SA {
public:
  SA(const PfspInstance& instance, double initial_temp = -1.0,
     double cooling_rate = 0.999, int iterations = 10000, int log_interval = 1);

  // Estimates T0 so that worsening moves are accepted with probability p0.
  // Uses: T0 = -avg_positive_delta / ln(p0)
  static double estimate_initial_temp(const PfspInstance& instance, double p0 = 0.8, int n_samples = 200);

  Solution run();
  SummaryStats runMultiple(int n_runs = 10);
  const std::vector<std::vector<Solution>>& getGenerationHistory() const { return history; }

private:
  const PfspInstance& instance;
  double initial_temp;
  double cooling_rate;
  int iterations;
  int log_interval;
  std::vector<std::vector<Solution>> history;
};
