#pragma once

#include <vector>
#include <string>

class PfspInstance {
public:
  int num_jobs;
  int num_machines;
  int seed;
  int up_bound;
  int low_bound;
  std::vector<std::vector<int>> processing_times; // processing_times[machine][job]

  PfspInstance(const std::string& filename);

private:
  void load_from_file(const std::string& filename);
};
