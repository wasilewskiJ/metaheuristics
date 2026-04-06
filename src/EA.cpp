#include "EA.hpp"
#include <random>
#include <stdexcept>

Solution selection_tournament(const std::vector<Solution>& population, int tour_size) {
  if (tour_size <= 0 || tour_size > (int)population.size())
    throw std::invalid_argument("tour_size must be in [1, population.size()]");

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, population.size() - 1);

  // Pick tour_size random individuals, track the best
  const Solution* best = &population[dist(gen)];
  for (int i = 1; i < tour_size; i++) {
    const Solution* candidate = &population[dist(gen)];
    if (candidate->total_time < best->total_time)
      best = candidate;
  }

  return *best;
}
