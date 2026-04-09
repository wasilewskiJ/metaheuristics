#pragma once

#include "PfspInstance.hpp"
#include <iostream>
#include <vector>

struct SummaryStats {
   int best;
   int worst;
   double avg;
   double std_dev;
};

SummaryStats compute_stats(const std::vector<Solution>& solutions);
SummaryStats compute_multirun_stats(const std::vector<int>& best_per_run);

class Logger {
public:
  std::vector<Solution> solutions;
  std::string output_filename;
  void dumpToFile();
  void printBestTime() const {
    std::cout << "Best Time: " << best_times.back() << std::endl;
  };
  Logger(std::vector<Solution> solutions, std::string output_filename)
      : solutions(solutions), output_filename(output_filename) {
    calculateTimes();
  };

  // EA constructor: computes best/avg/worst per generation
  Logger(std::vector<std::vector<Solution>> generations, std::string output_filename)
      : output_filename(output_filename) {
    calculateTimesPerGeneration(generations);
  };

  SummaryStats getSummaryStats() {
    return summaryStats;
  }

private:
  std::vector<int> best_times;
  std::vector<int> avg_course;
  std::vector<int> worst_times;
  std::vector<int> best_job_sequence;
  std::vector<int> worst_job_sequence;
  SummaryStats summaryStats;

  void calculateTimes();
  void calculateTimesPerGeneration(const std::vector<std::vector<Solution>>& generations);
};
