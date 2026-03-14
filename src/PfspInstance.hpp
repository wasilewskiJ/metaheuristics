#pragma once

#include <vector>
#include <string>
#include <iostream>

struct Solution {
  std::vector<int> job_sequence;
  int total_time;

  Solution(int total_time, const std::vector<int>& job_sequence) : total_time(total_time), job_sequence(job_sequence) {};
  void print() const {
    std::cout << "Total Time: " << total_time << "\nJob Sequence: ";
    for (int job : job_sequence) {
      std::cout << job << " ";
    }
    std::cout << std::endl;
  }
};

class PfspInstance {
public:
  int num_jobs;
  int num_machines;
  int seed;
  int up_bound;
  int low_bound;
  std::vector<std::vector<int>> processing_times; // processing_times[machine][job]

  PfspInstance(const std::string& filename);
  // randomAlgorithm returns a random solution for the problem instance
  Solution randomAlg() const;
private:
  void load_from_file(const std::string& filename);
  int calculate_total_time(const std::vector<int> &job_sequence) const;
};
