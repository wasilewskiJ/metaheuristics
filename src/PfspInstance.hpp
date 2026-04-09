#pragma once

#include <string>
#include <vector>

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
  std::vector<std::vector<int>>
      processing_times; // processing_times[machine][job]

  PfspInstance(const std::string &filename);
  // randomAlg returns a random solution for the problem instance
  Solution randomAlg() const;
  Solution greedyAlg(int starting_job) const;
  // runRandomAlg runs randomAlg() `iterations` times and returns aggregate
  // stats
  std::vector<Solution> runRandomAlg(int iterations = 1000) const;
  std::vector<Solution> runGreedyAlg(int starting_job = -1, int max_starts = 100) const;

  int calculate_total_time(const std::vector<int> &job_sequence) const;

  void reset_eval_counter() { eval_counter = 0; }
  int get_eval_counter() const { return eval_counter; }

private:
  mutable int eval_counter = 0;
  void load_from_file(const std::string &filename);
};
