#ifndef CPU_HPP_
#define CPU_HPP_

#include <array>
#include <cstdint>
#include "memory.hpp"
#include "instruction_parser.hpp"

namespace simulator {

class Cpu {
 private:
  static constexpr int kNumberOfRegirsters = 32;
  static constexpr std::size_t kInstrucionSize = 4;

  struct PiplelineData {
    std::uint32_t raw_instruction;
    Instruction instruction;

    std::uint32_t command_result;
    std::uint32_t memory_read_data;
  } pipeline_data_;

 public:
  Cpu(Memory& memory);

  void pipeline_cycle();

 private:
  void fetch();
  void decode();
  void execute();
  void write_back();
  void advance();

  std::uint32_t execute_rformat(Instruction& instruction);

  std::array<std::uint32_t, kNumberOfRegirsters> regirsters_ = {0};
  std::uint32_t program_counter_ = 0;

  Memory& memory_;
  std::uint32_t program_address_ = 0;
};

} // namespace simulator

#endif // CPU_HPP_
