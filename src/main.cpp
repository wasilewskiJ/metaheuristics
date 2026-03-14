#include "PfspInstance.hpp"

int main() {
  try {
    PfspInstance instance("0.fsp");
    Solution sol = instance.randomAlg();
    sol.print();
  } catch (const std::exception &e) {
    return 1;
  }
  return 0;
}
