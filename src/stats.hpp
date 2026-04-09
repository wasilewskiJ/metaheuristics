#pragma once
#include "PfspInstance.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

struct SummaryStats {
  int best;
  int worst;
  double avg;
  double std_dev;
};

// For random/greedy: stats over a flat list of solutions
inline SummaryStats compute_stats(const std::vector<Solution>& solutions) {
  int best  = std::numeric_limits<int>::max();
  int worst = std::numeric_limits<int>::min();
  double sum = 0;
  for (const auto& s : solutions) {
    if (s.total_time < best)  best  = s.total_time;
    if (s.total_time > worst) worst = s.total_time;
    sum += s.total_time;
  }
  double avg = sum / solutions.size();
  double var = 0;
  for (const auto& s : solutions)
    var += (s.total_time - avg) * (s.total_time - avg);
  double std_dev = std::sqrt(var / solutions.size());
  return {best, worst, avg, std_dev};
}

// For EA/SA: stats over best-per-run values
inline SummaryStats compute_multirun_stats(const std::vector<int>& best_per_run) {
  int best  = *std::min_element(best_per_run.begin(), best_per_run.end());
  int worst = *std::max_element(best_per_run.begin(), best_per_run.end());
  double sum = 0;
  for (int v : best_per_run) sum += v;
  double avg = sum / best_per_run.size();
  double var = 0;
  for (int v : best_per_run)
    var += (v - avg) * (v - avg);
  double std_dev = std::sqrt(var / best_per_run.size());
  return {best, worst, avg, std_dev};
}
