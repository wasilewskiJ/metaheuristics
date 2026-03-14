#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <string>

#include "PfspInstance.hpp"

#define TEST_CASES_DIR "../test_cases/"
constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

PfspInstance::PfspInstance(const std::string &filename) {
  load_from_file(filename);
}

void PfspInstance::load_from_file(const std::string &filename) {
  std::string path = TEST_CASES_DIR + filename;
#if DEBUG_LOADER
  std::cout << "DEBUG: Attempting to load test case from file: " << path
            << std::endl;
#endif

  std::ifstream infile(path);
  if (!infile) {
    std::cerr << "ERROR: Could not open file: " << path << std::endl;
    throw std::runtime_error("File open failed");
  }
  infile.ignore(max_size, '\n'); // Skip the first line
  infile >> num_jobs >> num_machines >> seed >> up_bound >> low_bound;
#if DEBUG_LOADER
  std::cout << "DEBUG: Loaded problem parameters - num_jobs: " << num_jobs
            << ", num_machines: " << num_machines << ", seed: " << seed
            << ", up_bound: " << up_bound << ", low_bound: " << low_bound
            << std::endl;
#endif

  infile.ignore(max_size, ':'); // Skip the third line
  processing_times.clear();
  for (int m = 0; m < num_machines; m++) {
    processing_times.push_back(std::vector<int>());
    for (int j = 0; j < num_jobs; j++) {
      int processing_time;
      infile >> processing_time;
      processing_times[m].push_back(processing_time);
#if DEBUG_LOADER
      std::cout << processing_times[m][j] << ", ";
#endif
    }
#if DEBUG_LOADER
    std::cout << std::endl;
#endif
  }
}

int PfspInstance::calculate_total_time(const std::vector<int> &job_sequence) const {
  std::vector<int> machine_completion_times(num_machines, 0);
  for (int job : job_sequence) {
    for (int m = 0; m < num_machines; m++) {
      int processing_time = processing_times[m][job];
      machine_completion_times[m] += processing_time;
    }
  }
  return *machine_completion_times.end();
}

Solution PfspInstance::randomAlg() const {
  std::vector<int> job_sequence(num_jobs);

  for (int i = 0; i < num_jobs; ++i) {
    job_sequence[i] = i;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(job_sequence.begin(), job_sequence.end(), gen);
  int total_time = calculate_total_time(job_sequence);
  return Solution(total_time, job_sequence);
};
