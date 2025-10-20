#include <cstddef>
#include <iostream>
#include "interactive_simulator.hpp"

constexpr std::size_t kInitialMemSize = 2048;

int main() {
  simulator::InteractiveSimulator simulator(kInitialMemSize);
  simulator.start();
  return 0;
}
