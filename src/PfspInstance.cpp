#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
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
    throw std::runtime_error("File open failed: " + path);
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

/*
int PfspInstance::calculate_total_time(
    const std::vector<int> &job_sequence) const {
  std::vector<int> job_times_on_last_machine(num_jobs, 0);
  for (auto it = job_sequence.begin(); it != job_sequence.end(); ++it) {
    int time_on_last_machine = 0;
    for (int m = 0; m < num_machines; m++) {
      time_on_last_machine += processing_times[m][*it];
      if (it != job_sequence.begin())
        time_on_last_machine += processing_times[m][*(it - 1)];
    }
    job_times_on_last_machine[*it] = time_on_last_machine;
    std::cout << "JOB: " << *it + 1
              << " Time on last machine: " << time_on_last_machine << std::endl;
  }
  int sum = 0;
  for(int time : job_times_on_last_machine) {
    sum += time;
  }
  return sum;
}
*/ 
/*
job_time_finish_on_machine[m][j];
job_time_finish_on_machine[0][jobs_sequence[0]] = processing_times[0][jobs_seq[0]];
for 1..j:
  job_time_finish_on_machine[0][j] += processing_times[0][j] + job_time_finish[0][j-1];


for 1..m:
  time = 0
  for 0..j:
    if (time < job_time_finish_on_machine[m-1][j])
      time += (uint)(processing_times[m-1][j] - time)
      job_time_finish_on_machine[m][j] = time + processing_time[m][j];
 */     


int PfspInstance::calculate_total_time(const std::vector<int> &job_sequence) const {
  int job_time_finish_on_machine[num_machines][num_jobs];
  job_time_finish_on_machine[0][job_sequence[0]] = processing_times[0][job_sequence[0]];
  for (auto it = (job_sequence.begin() + 1); it != job_sequence.end(); ++it) {
    job_time_finish_on_machine[0][*it] = processing_times[0][*it];
    job_time_finish_on_machine[0][*it] += job_time_finish_on_machine[0][*(it-1)];
  }  
  
  int time_spent_on_machine[num_machines];
  for (int m = 1; m < num_machines; m++) {
    int time = 0;
    for (auto it = job_sequence.begin(); it != job_sequence.end(); ++it) {
      if(time < job_time_finish_on_machine[m-1][*it]) {
        time = job_time_finish_on_machine[m-1][*it];
      } 
        time += processing_times[m][*it];
        job_time_finish_on_machine[m][*it] = time;
    }
    time_spent_on_machine[m] = time;
  }
  int sum = 0;
  for (int i = 0; i < num_jobs; i++) {
    sum += job_time_finish_on_machine[num_machines-1][i];
  }
  return sum;
}

/*
int PfspInstance::calculate_total_time(const std::vector<int> &job_sequence)
const { std::vector<int> machine_times(num_machines, 0); for (int job :
job_sequence) { for (int m = 0; m < num_machines; m++) { machine_times[m] +=
processing_times[m][job];
    }
  }
  return *std::max_element(machine_times.begin(), machine_times.end());
*/

std::vector<Solution> PfspInstance::runRandomAlg(int iterations) const {
  if (iterations <= 0) {
    throw std::invalid_argument("iterations must be greater than 0");
  }

  // return all results in a vector
  std::vector<Solution> results;
  for (int i = 0; i < iterations; ++i) {
    results.push_back(randomAlg());
  }

#if DEBUG_RANDOM_ALG
  std::cout << "RandomAlg: number of iterations: " << iterations << std::endl;
  for (auto &solution : results) {
    std::cout << solution.total_time << std::endl;
  }
#endif

  return results;
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
