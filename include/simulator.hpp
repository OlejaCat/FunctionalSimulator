#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "cpu.hpp"
#include "memory.hpp"

namespace simulator{

class Simulator {
 public:
  Simulator(std::size_t memory_size);

  void load_program(const std::uint8_t* program, std::size_t size);

 private:
  Memory memory_;
  Cpu cpu_;
};
} // simulator


#endif // SIMULATOR_HPP
