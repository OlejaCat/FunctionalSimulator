#include "cpu.hpp"
#include <sys/types.h>
#include <cstdint>
#include <ios>
#include <iostream>

#include "instruction_formats.hpp"
#include "instruction_parser.hpp"
#include "opcodes.hpp"
#include "bit_shifts.hpp"
#include "syscalls.hpp"


namespace simulator {

Cpu::Cpu(Memory& memory) 
  : memory_(memory) {}

std::uint32_t Cpu::get_pc() const {
  return program_counter_;
}

void Cpu::set_pc(std::uint32_t program_counter) {
  program_counter_ = program_counter;
}

std::uint32_t Cpu::get_register(std::uint8_t index) const {
  return registers_[index];
}


void Cpu::set_register(std::uint8_t index, std::uint32_t data) {
  registers_[index] = data;
}


void Cpu::run_program() {
  should_run_ = true; 

  while (should_run_) {
    pipeline_cycle();
  }
}

void Cpu::pipeline_cycle() {
  fetch();
  decode();
  execute();
  write_back();
  advance();
}

void Cpu::fetch() {
  std::uint32_t raw_instruction = memory_.read_word(program_counter_);
  pipeline_data_.raw_instruction = raw_instruction;
  pipeline_data_.next_program_counter = program_counter_ + kInstrucionSize;
}

void Cpu::decode() {
  pipeline_data_.instruction = InstructionParser::parse(pipeline_data_.raw_instruction);
}

void Cpu::execute() {
  Instruction& instruction = pipeline_data_.instruction;

  switch(instruction.opcode) {
    case opcodes::kNOR:
    case opcodes::kADD:
    case opcodes::kXOR:
      pipeline_data_.command_result = execute_rformat();
      break;

    case opcodes::kLD:
    case opcodes::kST:
      pipeline_data_.command_result = execute_memformat();
      break;

    case opcodes::kBEQ:
    case opcodes::kBNE:
      execute_branch_format();
      break;

    case opcodes::kCBIT:
    case opcodes::kSSAT:
      pipeline_data_.command_result = execute_rd_rs_imm5_format();
      break;

    case opcodes::kCLZ:
      pipeline_data_.command_result = execute_clz_format();
      break;

    case opcodes::kBDEP:
      pipeline_data_.command_result = execute_bdep_format();
      break;

    case opcodes::kLDP:
      execute_ldp_format();
      break;

    case opcodes::kJj:
      execute_j_format();
      break;

    case opcodes::kSYSCALL:
      execute_syscall_format();
      break;

    default:
      pipeline_data_.raw_instruction = 0;
      break;
  }
}

void Cpu::write_back() {
  if (pipeline_data_.raw_instruction == 0) {
    return;
  }

  Instruction& instruction = pipeline_data_.instruction;

  std::uint8_t destination_register = 0;
  switch (instruction.opcode) {
    case opcodes::kNOR:
    case opcodes::kADD:
    case opcodes::kXOR:
      destination_register = std::get<RFormat>(instruction.fields).rd;
      break;

    case opcodes::kLD:
      destination_register = std::get<MemBaseRtOffset16Format>(instruction.fields).rt;
      break;

    case opcodes::kCBIT:
    case opcodes::kSSAT:
      destination_register = std::get<RdRsImm5Format>(instruction.fields).rd;
      break;

    case opcodes::kCLZ:
      destination_register = std::get<ClzFormat>(instruction.fields).rd;
      break;

    case opcodes::kBDEP:
      destination_register = std::get<BdepFormat>(instruction.fields).rd;
      break;

    default:
      break;
  }

  if (destination_register != 0 && destination_register < kNumberOfRegirsters) {
    registers_[destination_register] = pipeline_data_.command_result;
  }
}

void Cpu::advance() {
  program_counter_ = pipeline_data_.next_program_counter;
}


void Cpu::print_registers() const {
  std::cout << "PC: 0x" << std::hex << program_counter_ << "\n";
  for (std::size_t i = 0; i < kNumberOfRegirsters; ++i) {
    std::cout << "R" << std::dec << i << ": 0x" << std::hex << registers_[i] << "\n";
  }
  std::cout << std::dec << std::endl;
}


std::int32_t Cpu::sign_extend(std::uint16_t value) {
  std::int16_t sign_value_16_bit = static_cast<std::int16_t>(value);
  std::int32_t sign_value_32_bit = static_cast<std::int32_t>(sign_value_16_bit);
  return sign_value_32_bit;
}


std::int32_t Cpu::calculate_branch_target(std::uint16_t offset) {
  return sign_extend(offset) << 2;
}

std::uint32_t Cpu::clear_bit_field(std::uint32_t value, std::uint8_t index) {
  if (index >= kNumberOfBitsInWord) {
    return value;
  }
  return value & ~(1U << index);
}

std::uint32_t Cpu::saturate_signed(std::uint32_t value, std::uint8_t number) {
  if (number == 0 || number > kNumberOfBitsInWord) {
    return value;
  }

  std::int32_t signed_value = static_cast<std::int32_t>(value);
  std::int32_t max_value = (1 << (value - 1)) - 1;
  std::int32_t min_value = -(1 << (value - 1));

  if (signed_value > max_value) {
    return static_cast<std::uint32_t>(max_value);
  } 
  if (signed_value < min_value) {
    return static_cast<std::uint32_t>(min_value);
  }
  return value;
}

std::uint32_t Cpu::count_leading_zeros(std::uint32_t value) {
  if (value == 0) {
    return kNumberOfBitsInWord;
  }

  std::uint32_t count = 0;
  for (std::uint32_t i = kNumberOfBitsInWord; i > 0; --i) {
    if (((value >> (i - 1)) & 1) != 0) {
      break;
    }
    ++count;
  }
  return count;
}

std::uint32_t Cpu::bit_deposit(std::uint32_t value, std::uint32_t mask) {
  std::uint32_t result = 0;
  std::uint32_t value_bit = 0;
  for (std::uint32_t i = 0; i < kNumberOfBitsInWord; ++i) {
    if (((mask >> i) & 1) != 0) {
      if (((value >> value_bit) & 1) != 0) {
        result |= (1U << i);
      }
      ++value_bit;
    }
  }
  return result;
}

std::uint32_t Cpu::execute_rformat() {
  const auto& format = std::get<RFormat>(pipeline_data_.instruction.fields);
  std::uint32_t rs_data = registers_[format.rs];
  std::uint32_t rt_data = registers_[format.rt];
  switch (pipeline_data_.instruction.opcode) {
    case opcodes::kNOR: 
      return ~(rs_data | rt_data);
    case opcodes::kADD: 
      return rs_data + rt_data;
    case opcodes::kXOR:
      return rs_data ^ rt_data;
    default:
      return 0;
  }
} 

std::uint32_t Cpu::execute_memformat() {
  const auto& format = std::get<MemBaseRtOffset16Format>(pipeline_data_.instruction.fields);
  std::uint32_t address = registers_[format.base] + sign_extend(format.offset);
  
  switch (pipeline_data_.instruction.opcode) {
    case opcodes::kLD:
      pipeline_data_.memory_read_data = memory_.read_word(address);
      return pipeline_data_.memory_read_data;
    case opcodes::kST:
      memory_.write_word(address, registers_[format.rt]);
      return 0;
    default:
      return 0;
  }
}

void Cpu::execute_branch_format() {
  const auto& format = std::get<BranchRsRtOffset16Format>(pipeline_data_.instruction.fields);
  bool condition = false;
  if (pipeline_data_.instruction.opcode == opcodes::kBNE) {
    condition = (registers_[format.rs] != registers_[format.rt]);
  } else if (pipeline_data_.instruction.opcode == opcodes::kBEQ) {
    condition = (registers_[format.rs] == registers_[format.rt]);
  }

  if (condition) {
    std::int32_t target = calculate_branch_target(format.offset);
    pipeline_data_.next_program_counter = program_counter_ + target;
  }

  pipeline_data_.raw_instruction = 0;
}

std::uint32_t Cpu::execute_rd_rs_imm5_format() {
  const auto& format = std::get<RdRsImm5Format>(pipeline_data_.instruction.fields);
  std::uint32_t rs_data = registers_[format.rs];
  switch (pipeline_data_.instruction.opcode) {
    case opcodes::kCBIT:
      return clear_bit_field(rs_data, format.imm5);
    case opcodes::kSSAT:
      return saturate_signed(rs_data, format.imm5);
    default:
      return 0;
  }
}

std::uint32_t Cpu::execute_clz_format() {
  const auto& format = std::get<ClzFormat>(pipeline_data_.instruction.fields);
  return count_leading_zeros(registers_[format.rs]);
}

std::uint32_t Cpu::execute_bdep_format() {
  const auto& format = std::get<BdepFormat>(pipeline_data_.instruction.fields);
  return bit_deposit(registers_[format.rs1], registers_[format.rs2]);
}

void Cpu::execute_ldp_format() {
  const auto& format = std::get<LdpFormat>(pipeline_data_.instruction.fields);
  std::uint32_t address = registers_[format.base] + sign_extend(format.offset);

  registers_[format.rt1] = memory_.read_word(address);
  registers_[format.rt2] = memory_.read_word(address + kInstrucionSize);

  pipeline_data_.raw_instruction = 0;
} 

void Cpu::execute_j_format() {
  const auto& format = std::get<JTarget26Format>(pipeline_data_.instruction.fields);
  std::uint32_t pc_upper_4_bits = program_counter_ & shifts::kFirst4BitsMask;
  pipeline_data_.next_program_counter = pc_upper_4_bits | (format.target_index << 2);
  pipeline_data_.raw_instruction = 0;
}

void Cpu::execute_syscall_format() {
  std::uint32_t syscall_number = registers_[syscalls::kNumberRegister];
  
  std::uint32_t result = 0;
  switch (syscall_number) {
    case syscalls::EXIT:
      should_run_ = false;
      break;
  }

  registers_[syscalls::kResult] = result;

}

} // namespace simulator
 

