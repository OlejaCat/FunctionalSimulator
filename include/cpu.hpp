#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>
#include "memory.hpp"

namespace simulator {

class Cpu {
 private:
  static constexpr int kNumberOfRegirsters = 32;

 public:
  Cpu(const Memory& memory);

  void pipeline_cycle();

 private:
  void fetch();
  void decode();
  void execute();
  void write();
  void advance();

  std::array<std::uint32_t, kNumberOfRegirsters> regirsters_ = {0};
  std::uint32_t program_counter_ = 0;

  Memory& memory_;
};

} // namespace simulator

#endif // CPU_HPP
