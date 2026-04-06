#include "operators.hpp"

#include <random>
#include <algorithm>
#include <unordered_set>

std::vector<int> mutate_swap(std::vector<int> job_sequence) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, job_sequence.size()-1);
  int a = dist(gen); 
  int b;
  do { b = dist(gen); } while (b == a);
  std::swap(job_sequence[a], job_sequence[b]); 
  return job_sequence;
}

std::vector<int> mutate_inversion(std::vector<int> job_sequence) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, job_sequence.size() - 1);
  int a = dist(gen);
  int b;
  do { b = dist(gen); } while (b == a);
  if (a > b) std::swap(a, b);
  // a and b are now the bounds of the subsequence to invert (a < b)
  std::reverse(job_sequence.begin() + a, job_sequence.begin() + b + 1);
  return job_sequence;
}

std::vector<int> crossover_ox(const std::vector<int>& p1, const std::vector<int>& p2) {
  int n = p1.size();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, n - 1);

  int a = dist(gen);
  int b;
  do { b = dist(gen); } while (b == a);
  if (a > b) std::swap(a, b);

  // Step 1: copy segment [a, b] from p1 into child
  std::vector<int> child(n, -1);
  std::unordered_set<int> in_child;
  for (int i = a; i <= b; i++) {
    child[i] = p1[i];
    in_child.insert(p1[i]);
  }

  // Step 2: scan p2 from index 0, skip values already in child,
  // fill remaining empty slots left to right
  int child_pos = 0;
  for (int i = 0; i < n; i++) {
    if (!in_child.count(p2[i])) {
      // advance child_pos to next empty slot
      while (child_pos < n && child[child_pos] != -1)
        child_pos++;
      child[child_pos] = p2[i];
      in_child.insert(p2[i]);
    }
  }

  return child;
}
