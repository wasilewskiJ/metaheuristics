#pragma once
#include "operators.hpp"
#include "PfspInstance.hpp"

// Randomly picks tour_size individuals from population, returns the best one
Solution selection_tournament(const std::vector<Solution>& population, int tour_size);
