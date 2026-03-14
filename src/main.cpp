#include "PfspInstance.hpp"

int main() {
  try {
    PfspInstance instance("0.fsp");
  } catch (const std::exception &e) {
    return 1;
  }
  return 0;
}
