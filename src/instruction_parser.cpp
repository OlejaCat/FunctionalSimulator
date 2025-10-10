#include "instruction_parser.hpp"
#include <stdexcept>
#include <string>

namespace simulator {

std::uint8_t InstructionParser::get_opcode(std::uint32_t instruction) {
  std::uint8_t primary_opcode = (instruction >> kOpcodeShift) & kOpcodeMask;
  if (primary_opcode != 0) {
    return primary_opcode;
  }
  return static_cast<std::uint8_t>(instruction & kOpcodeMask);
}

Instruction InstructionParser::parse(std::uint32_t raw_instruction) {
  Instruction instruction;
  instruction.raw = raw_instruction;
  instruction.opcode = get_opcode(raw_instruction);

  using namespace shifts;

  switch (instruction.opcode) {
    case opcodes::kNOR:
    case opcodes::kADD:
    case opcodes::kXOR:
      {
        RFormat format;
        format.rd = (raw_instruction >> k11BitShift) & k5BitMask;
        format.rt = (raw_instruction >> k16BitShift) & k5BitMask;
        format.rs = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kBDEP:
      {
        BdepFormat format;
        format.rs2 = (raw_instruction >> k11BitShift) & k5BitMask;
        format.rs1 = (raw_instruction >> k16BitShift) & k5BitMask;
        format.rd  = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kCLZ:
      {
        ClzFormat format;
        format.rs = (raw_instruction >> k16BitShift) & k5BitMask;
        format.rd = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kCBIT:
    case opcodes::kSSAT:
      {
        RdRsImm5Format format;
        format.imm5 = (raw_instruction >> k11BitShift) & k5BitMask;
        format.rs = (raw_instruction >> k16BitShift) & k5BitMask; 
        format.rd = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kST:
    case opcodes::kLD:
      {
        MemBaseRtOffset16Format format;
        format.offset = raw_instruction & k16BitMask;
        format.rt = (raw_instruction >> k16BitShift) & k5BitMask;
        format.base = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kBNE:
    case opcodes::kBEQ:
      {
        BranchRsRtOffset16Format format;
        format.offset = raw_instruction & k16BitMask;
        format.rt = (raw_instruction >> k16BitShift) & k5BitMask;
        format.rs = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kLDP:
      {
        LdpFormat format;
        format.offset = raw_instruction & k11BitMask;
        format.rt2 = (raw_instruction >> k11BitShift) & k5BitMask;
        format.rt1 = (raw_instruction >> k16BitShift) & k5BitMask;
        format.base = (raw_instruction >> k21BitShift) & k5BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kJj:
      {
        JTarget26Format format;
        format.target_index = raw_instruction & k26BitMask;
        instruction.fields = format;
        break;
      }
    case opcodes::kSYSCALL:
      {
        SyscallFormat format;
        format.code = (raw_instruction >> k6BitShift) & k20BitMask;
        instruction.fields = format;
        break;
      }
    default:
      throw std::runtime_error("Unknown opcode: " + std::to_string(instruction.opcode));
  }

  return instruction;
}

} // namespace simulator
