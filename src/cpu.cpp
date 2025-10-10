#include "cpu.hpp"
#include <sys/types.h>
#include "instruction_parser.hpp"


namespace simulator {

Cpu::Cpu(Memory& memory) 
  : memory_(memory) {}

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
      pipeline_data_.command_result = execute_rformat(instruction);
      break;

    default:
      break;
  }
  // maybe add executer for that purpose
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

    default:
      break;
  }

  if (destination_register < kNumberOfRegirsters) {
    regirsters_[destination_register] = pipeline_data_.command_result;
  }
}

void Cpu::advance() {
  program_counter_ += kInstrucionSize;
}

std::uint32_t Cpu::execute_rformat(Instruction& instruction) {
  const auto& format = std::get<RFormat>(instruction.fields);
  std::uint32_t rs_data = regirsters_[format.rs];
  std::uint32_t rt_data = regirsters_[format.rt];
  switch (instruction.opcode) {
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

} // namespace simulator
 

