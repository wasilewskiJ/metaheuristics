#pragma once
#include "operators.hpp"
#include "PfspInstance.hpp"
#include <vector>

Solution selection_tournament(const std::vector<Solution>& population, int tour_size);

enum class CrossoverType { OX, PMX, CX };
enum class MutationType  { SWAP, INVERSION };

class EA {
public:
  EA(const PfspInstance& instance,
     int pop_size    = 100,
     int generations = 100,
     float Px        = 0.7,
     float Pm        = 0.1,
     int tour_size   = 5,
     CrossoverType crossover = CrossoverType::OX,
     MutationType  mutation  = MutationType::SWAP);

  Solution run();
  const std::vector<std::vector<Solution>>& getGenerationHistory() const { return generation_history; }

private:
  const PfspInstance& instance;
  int pop_size;
  int generations;
  int tour_size;
  float Px;
  float Pm;
  CrossoverType crossover;
  MutationType  mutation;
  std::vector<Solution> population;
  std::vector<std::vector<Solution>> generation_history;

  void initialize_population();
};
