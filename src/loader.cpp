#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#define TEST_CASES_DIR "../test_cases/"
constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

struct Problem {
  int num_jobs;
  int num_machines;
  int seed;
  int up_bound;
  int low_bound;
  std::vector<std::vector<int>>
      processing_times; // processing_times[machine][job]
};

/* Example test case:

 number of jobs, number of machines, initial seed, upper bound and lower bound :
 4 5 - - -
processing times :
30 29 19 50
39 69 6 73
 3 86 58 60
72 74 51 15
24 47 22 90

*/

bool load_test_case(const char *filename, Problem &problem) {
  std::string path = TEST_CASES_DIR + std::string(filename);
#if DEBUG_LOADER
  std::cout << "DEBUG: Attempting to load test case from file: " << path
            << std::endl;
#endif

  std::ifstream infile(path);
  if (!infile) {
    std::cerr << "ERROR: Could not open file: " << path << std::endl;
  }
  infile.ignore(max_size, '\n'); // Skip the first line
  infile >> problem.num_jobs >> problem.num_machines >> problem.seed >>
      problem.up_bound >> problem.low_bound;
#if DEBUG_LOADER
  std::cout << "DEBUG: Loaded problem parameters - num_jobs: "
            << problem.num_jobs << ", num_machines: " << problem.num_machines
            << ", seed: " << problem.seed << ", up_bound: " << problem.up_bound
            << ", low_bound: " << problem.low_bound << std::endl;
#endif

  infile.ignore(max_size, ':'); // Skip the third line
  for (int m = 0; m < problem.num_machines; m++) {
    problem.processing_times.push_back(std::vector<int>());
    for (int j = 0; j < problem.num_jobs; j++) {
      int processing_time;
      infile >> processing_time;
      problem.processing_times[m].push_back(processing_time);
      std::cout << problem.processing_times[m][j] << ", ";
    }
    std::cout << std::endl;
  }

  return true;
}

int main() {
  Problem problem;
  if (load_test_case("0.fsp", problem)) {
    return 1;
  }
  return 0;
}
