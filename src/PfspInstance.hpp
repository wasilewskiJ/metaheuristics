#pragma once

#include <string>
#include <vector>

/*
struct LoggerResult {
  int num_iterations;
  std::vector<int> best_times;
  std::vector<int> avg_course;
  std::vector<int> worst_times;

  std::vector<int> best_job_sequence;
  std::vector<int> worst_job_sequence;

  void print() const {
    std::cout << "Best Time: " << *best_times.end()
              << "\nWorst Time: " << *worst_times.end() << "\nBest Job Sequence: ";
    for (int job : best_job_sequence) {
      std::cout << job << " ";
    }
    std::cout << std::endl;
  }
};
*/ 

struct Solution {
  int total_time;
  std::vector<int> job_sequence;

  Solution(int time, const std::vector<int> &sequence)
      : total_time(time), job_sequence(sequence) {}
};

class PfspInstance {
public:
  int num_jobs;
  int num_machines;
  int seed;
  int up_bound;
  int low_bound;
  std::vector<std::vector<int>> processing_times; // processing_times[machine][job]

  PfspInstance(const std::string &filename);
  // randomAlg returns a random solution for the problem instance
  Solution randomAlg() const;
  // runRandomAlg runs randomAlg() `iterations` times and returns aggregate
  // stats
  std::vector<Solution> runRandomAlg(int iterations = 1000) const;

private:
  void load_from_file(const std::string &filename);
  int calculate_total_time(const std::vector<int> &job_sequence) const;
};
