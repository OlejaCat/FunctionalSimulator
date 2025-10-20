#ifndef CPU_HPP_
#define CPU_HPP_

#include <array>
#include <cstdint>
#include "memory.hpp"
#include "instruction_formats.hpp"

namespace simulator {

class Cpu {
 private:
  static constexpr std::size_t kNumberOfRegirsters = 32;
  static constexpr std::uint32_t kNumberOfBitsInWord = 32;
  static constexpr std::size_t kInstrucionSize = 4;

  static constexpr std::size_t kMaxCycles = 1000;

  struct PiplelineData {
    std::uint32_t raw_instruction;
    Instruction instruction;

    std::uint32_t command_result;
    std::uint32_t memory_read_data;

    std::int32_t next_program_counter;
  } pipeline_data_;

 public:
  Cpu(Memory& memory);

  std::uint32_t get_pc() const;
  void set_pc(std::uint32_t program_counter);

  std::uint32_t get_register(std::uint8_t index) const;
  void set_register(std::uint8_t index, std::uint32_t data);

  void run_program();
  void pipeline_cycle();

  void print_registers() const;

 private:
  void fetch();
  void decode();
  void execute();
  void write_back();
  void advance();

  static std::int32_t sign_extend(std::uint16_t value);
  static std::int32_t calculate_branch_target(std::uint16_t offset);
  static std::uint32_t clear_bit_field(std::uint32_t value, std::uint8_t index);
  static std::uint32_t saturate_signed(std::uint32_t value, std::uint8_t number);
  static std::uint32_t count_leading_zeros(std::uint32_t value);
  static std::uint32_t bit_deposit(std::uint32_t value, std::uint32_t mask);

  std::uint32_t execute_rformat();
  std::uint32_t execute_memformat();
  void execute_branch_format();
  std::uint32_t execute_rd_rs_imm5_format();
  std::uint32_t execute_clz_format();
  std::uint32_t execute_bdep_format();
  void execute_ldp_format();
  void execute_j_format();
  void execute_syscall_format();

  std::array<std::uint32_t, kNumberOfRegirsters> regirsters_ = {0};
  std::int32_t program_counter_ = 0;

  Memory& memory_;
  std::uint32_t program_address_ = 0;

  bool should_run_ = false;
};

} // namespace simulator

#endif // CPU_HPP_
