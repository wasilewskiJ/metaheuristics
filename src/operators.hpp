#pragma once
#include <vector>

// Randomly swaps two jobs in the sequence
std::vector<int> mutate_swap(std::vector<int> job_sequence);

// Reverses a random subsequence in the sequence
std::vector<int> mutate_inversion(std::vector<int> job_sequence);

// Ordered Crossover - copies a substring from p1, fills remainder in order from p2
std::vector<int> crossover_ox(const std::vector<int>& p1, const std::vector<int>& p2);

// Partially Matched Crossover - maps a segment between parents, fills rest by mapping
std::vector<int> crossover_pmx(const std::vector<int>& p1, const std::vector<int>& p2);

// Cycle Crossover - builds offspring based on cycles between parents
std::vector<int> crossover_cx(const std::vector<int>& p1, const std::vector<int>& p2);
