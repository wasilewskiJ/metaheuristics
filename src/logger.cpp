#include "logger.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>

SummaryStats compute_stats(const std::vector<Solution>& solutions) {
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
  return {best, worst, avg, std::sqrt(var / solutions.size())};
}

SummaryStats compute_multirun_stats(const std::vector<int>& best_per_run) {
  int best  = *std::min_element(best_per_run.begin(), best_per_run.end());
  int worst = *std::max_element(best_per_run.begin(), best_per_run.end());
  double sum = 0;
  for (int v : best_per_run) sum += v;
  double avg = sum / best_per_run.size();
  double var = 0;
  for (int v : best_per_run)
    var += (v - avg) * (v - avg);
  return {best, worst, avg, std::sqrt(var / best_per_run.size())};
}

void Logger::calculateTimes() {
  int best_time = std::numeric_limits<int>::max();
  int worst_time = std::numeric_limits<int>::min();
  long long sum = 0;
  int count = 0;
  for (const auto &solution : solutions) {
    if (solution.total_time < best_time) {
      best_time = solution.total_time;
    }
    if (solution.total_time > worst_time) {
      worst_time = solution.total_time;
    }
    sum += solution.total_time;
    ++count;
    best_times.push_back(best_time);
    worst_times.push_back(worst_time);
    avg_course.push_back(static_cast<int>(sum / count));
  }
  summaryStats.avg = (double) sum / count;
  summaryStats.best = best_times.back();
  summaryStats.worst = worst_times.back();
}

void Logger::calculateTimesPerGeneration(const std::vector<std::vector<Solution>>& generations) {
  for (const auto& generation : generations) {
    int best_time  = std::numeric_limits<int>::max();
    int worst_time = std::numeric_limits<int>::min();
    long long sum  = 0;
    for (const auto& s : generation) {
      if (s.total_time < best_time)  best_time  = s.total_time;
      if (s.total_time > worst_time) worst_time = s.total_time;
      sum += s.total_time;
    }
    best_times.push_back(best_time);
    worst_times.push_back(worst_time);
    avg_course.push_back(static_cast<int>(sum / generation.size()));
  }
}

void Logger::dumpToFile() {
  std::ofstream ofile(output_filename);
  for (int i = 0; i < static_cast<int>(best_times.size()); i++) {
    ofile << i + 1 << ";" << best_times[i] << ";" << avg_course[i] << ";"
          << worst_times[i] << ";;;\n";
  }
}
