#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "cpu.hpp"
#include "memory.hpp"

namespace simulator {

class Simulator {
 public:
  Simulator(std::size_t memory_size);

  Cpu& get_cpu() { return cpu_; }
  const Cpu& get_cpu() const { return cpu_; }

  Memory& get_memory() { return memory_; }
  const Memory& get_memory() const { return memory_; }

  void load_program(const std::vector<std::uint8_t>& program);

  void load_program(const std::uint8_t* program, std::size_t size);

 private:
  Memory memory_;
  Cpu cpu_;
};
} // namespace simulator

#endif // SIMULATOR_HPP
