#include "logger.hpp"

#include <fstream>
#include <limits>

void Logger::calculateTimes() {
  int best_time = std::numeric_limits<int>::max();
  int worst_time = std::numeric_limits<int>::min();
  long long sum = 0;
  int count = 0;
  for (const auto &solution : solutions) {
    if (solution.total_time < best_time) {
      best_time = solution.total_time;
    }
    if (solution.total_time > worst_time) {
      worst_time = solution.total_time;
    }
    sum += solution.total_time;
    ++count;
    best_times.push_back(best_time);
    worst_times.push_back(worst_time);
    avg_course.push_back(static_cast<int>(sum / count));
  }
}

void Logger::dumpToFile() {
  std::ofstream ofile(output_filename);
  for (int i = 0; i < static_cast<int>(best_times.size()); i++) {
    ofile << i + 1 << ";" << best_times[i] << ";" << avg_course[i] << ";"
          << worst_times[i] << ";;;\n";
  }
}
