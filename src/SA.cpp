#include "SA.hpp"
#include <cmath>
#include <random>

SA::SA(const PfspInstance& instance, double initial_temp, double cooling_rate, int iterations)
    : instance(instance), initial_temp(initial_temp),
      cooling_rate(cooling_rate), iterations(iterations) {}

Solution SA::run() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<double> prob(0.0, 1.0);

  Solution current = instance.randomAlg();
  Solution best = current;
  double temp = initial_temp;

  for (int i = 0; i < iterations; i++) {
    // Generate neighbour by swap mutation
    std::vector<int> neighbour_seq = mutate_swap(current.job_sequence);
    int neighbour_time = instance.calculate_total_time(neighbour_seq);
    Solution neighbour(neighbour_time, neighbour_seq);

    // Accept if better, or with probability e^(-delta/T) if worse
    int delta = neighbour_time - current.total_time;
    if (delta < 0 || prob(gen) < std::exp(-delta / temp))
      current = neighbour;

    if (current.total_time < best.total_time)
      best = current;

    // Cool down
    temp *= cooling_rate;

    // Log every 100 iterations
    if (i % 100 == 0)
      history.push_back({current});
  }

  return best;
}
