#include <fstream>
#include <iostream>
#include <limits>
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

int main() {
  try {
    PfspInstance instance("0.fsp");
  } catch (const std::exception &e) {
    return 1;
  }
  return 0;
}
