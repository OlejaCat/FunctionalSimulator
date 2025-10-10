#include "cpu.hpp"


// public --------------------------------------------------------------------------------

simulator::Cpu::Cpu(Memory& memory) 
  : memory_(memory) {}

void simulator::Cpu::pipeline_cycle() {
  fetch();
  decode();
  execute();
  write();
  advance();
}


// private ------------------------------------------------------------------------------

void simulator::Cpu::fetch() {
  std::uint32_t raw_instruction = memory_.read_word(program_counter_);
  pipeline_data_.raw_instruction = raw_instruction;
}

void simulator::Cpu::decode() {
  Instruction instruction = InstructionParser::parse(pipeline_data_.raw_instruction);
  pipeline_data_.instruction = instruction;
}


void simulator::Cpu::execute() {
  // maybe add executer for that purpose
}

void simulator::Cpu::write() {
  // some pipeline vars to do that
}

void simulator::Cpu::advance() {
  program_counter_ += kInstrucionSize;
}

