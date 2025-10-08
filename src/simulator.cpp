#include "simulator.hpp"

simulator::Simulator::Simulator(std::size_t memory_size)
    : memory_(Memory(memory_size)), cpu_(Cpu(memory_)) {}
