#include "simulator.hpp"
#include <cstdint>

namespace simulator {

Simulator::Simulator(std::size_t memory_size)
  : memory_(Memory(memory_size)), cpu_(Cpu(memory_)) {}


void Simulator::load_program(const std::vector<std::uint8_t>& program) {
  memory_.write_block(0, program.data(), program.size());
}

void Simulator::load_program(const std::uint8_t* program, std::size_t size) {
  memory_.write_block(0, program, size);
}

} // namespace simulator

