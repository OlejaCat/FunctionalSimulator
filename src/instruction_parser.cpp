#include "instruction_parser.hpp"
#include <stdexcept>

// public -------------------------------------------------------------------------------

simulator::Instruction simulator::InstructionParser::parse(std::uint32_t raw_instruction) {
  Instruction instruction;

  instruction.raw = raw_instruction;
  instruction.opcode = get_opcode(raw_instruction);

  instruction.type = determine_type(raw_instruction);
  
  switch (instruction.type) {
    case Instruction::RType:
      parse_rtype(instruction);
      break;

    case Instruction::IType:
      parse_itype(instruction);
      break;

    case Instruction::SType:
      parse_stype(instruction);
      break;

    case Instruction::UType:
      parse_utype(instruction);
      break;

    default:
      throw std::runtime_error("Unknown instruction type: " + std::to_string(instruction.type));
  }

  return instruction;
}

std::uint8_t simulator::InstructionParser::get_opcode(std::uint32_t instruction) {
  std::uint8_t first_six_bytes = (instruction >> kShiftToOpcode) & kEndOpcode;
  if (first_six_bytes != 0) {
    return first_six_bytes;
  }
  return static_cast<std::uint8_t>(instruction & kEndOpcode);
}

// private ------------------------------------------------------------------------------

simulator::Instruction::Type simulator::InstructionParser::determine_type(std::uint32_t instruction) {
  std::uint8_t opcode = get_opcode(instruction);
  return instructions_opcodes::kInstructionsMap.at(opcode);
}

void simulator::InstructionParser::parse_rtype(Instruction& instruction) {
  instruction.rd = (instruction.raw >> rtype::kRdShift) & kFiveBitMask;
  instruction.rt1 = (instruction.raw >> rtype::kRt1Shift) & kFiveBitMask;
  instruction.rs1 = (instruction.raw >> rtype::kRs1Shift) & kFiveBitMask;
}

void simulator::InstructionParser::parse_itype(Instruction& instruction) {
  instruction.immediate = (instruction.raw >> itype::kImm5Shift) & kFiveBitMask;
  instruction.rs1 = (instruction.raw >> itype::kRs1Shift) & kFiveBitMask;
  instruction.rd = (instruction.raw >> itype::kRdShift) & kFiveBitMask;
}

void simulator::InstructionParser::parse_stype(Instruction& instruction) {
  instruction.offset = instruction.raw & stype::kOffsetBitMask;
  instruction.rt1 = (instruction.raw >> stype::kRt1Shift) & kFiveBitMask;
  instruction.rt2 = (instruction.raw >> stype::kRt2Shift) & kFiveBitMask;
  instruction.base = (instruction.raw >> stype::kBaseShift) & kFiveBitMask;
}

void simulator::InstructionParser::parse_utype(Instruction& instruction) {
  instruction.immediate = (instruction.raw >> utype::kImmideateShift) & utype::kImmideateMask;
}
//  static void parse_utype(const Instruction& instruction);
