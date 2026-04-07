#include "EA.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>

#include "operators.hpp"

// --- Tournament selection ---

Solution selection_tournament(const std::vector<Solution>& population, int tour_size) {
  if (tour_size <= 0 || tour_size > (int)population.size())
    throw std::invalid_argument("tour_size must be in [1, population.size()]");

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, population.size() - 1);

  const Solution* best = &population[dist(gen)];
  for (int i = 1; i < tour_size; i++) {
    const Solution* candidate = &population[dist(gen)];
    if (candidate->total_time < best->total_time)
      best = candidate;
  }
  return *best;
}

// --- EA ---

EA::EA(const PfspInstance& instance, int pop_size, int generations,
       float Px, float Pm, int tour_size)
    : instance(instance), pop_size(pop_size), generations(generations),
      Px(Px), Pm(Pm), tour_size(tour_size) {}

void EA::initialize_population() {
  population.clear();
  for (int i = 0; i < pop_size; i++)
    population.push_back(instance.randomAlg());
}

Solution EA::run() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> prob(0.0f, 1.0f);

  initialize_population();
  generation_history.clear();

  Solution best_overall = *std::min_element(population.begin(), population.end(),
    [](const Solution& a, const Solution& b) { return a.total_time < b.total_time; });

  for (int g = 0; g < generations; g++) {
    std::vector<Solution> new_population;

    while ((int)new_population.size() < pop_size) {
      Solution p1 = selection_tournament(population, tour_size);
      Solution p2 = selection_tournament(population, tour_size);

      std::vector<int> child_seq;
      if (prob(gen) < Px)
        child_seq = crossover_ox(p1.job_sequence, p2.job_sequence);
      else
        child_seq = p1.job_sequence;

      if (prob(gen) < Pm)
        child_seq = mutate_swap(child_seq);

      int child_time = instance.calculate_total_time(child_seq);
      new_population.push_back(Solution(child_time, child_seq));
    }

    population = std::move(new_population);

    for (const auto& s : population)
      if (s.total_time < best_overall.total_time)
        best_overall = s;

    generation_history.push_back(population);
  }

  return best_overall;
}
