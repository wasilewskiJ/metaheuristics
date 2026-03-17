#pragma once

#include "PfspInstance.hpp"
#include <iostream>

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

private:
  std::vector<int> best_times;
  std::vector<int> avg_course;
  std::vector<int> worst_times;
  std::vector<int> best_job_sequence;
  std::vector<int> worst_job_sequence;
  void calculateTimes();
};
