#ifndef INSTRUCTION_PARSER_HPP_
#define INSTRUCTION_PARSER_HPP_

#include <cstdint>
#include <unordered_map>

namespace simulator {

namespace rtype {
  constexpr std::uint32_t kRdShift = 11;
  constexpr std::uint32_t kRt1Shift = 16;
  constexpr std::uint32_t kRs1Shift = 21;
} // namespace rtype

namespace itype {
  constexpr std::uint32_t kImm5Shift = 11; 
  constexpr std::uint32_t kRs1Shift = 16;
  constexpr std::uint32_t kRdShift = 21;
} // namespace utype

namespace stype {
  constexpr std::uint32_t kOffsetBitMask = 0x3F;
  constexpr std::uint32_t kRt2Shift = 11;
  constexpr std::uint32_t kRt1Shift = 16;
  constexpr std::uint32_t kBaseShift = 21;
} // namespace stype

namespace utype {
  constexpr std::uint32_t kImmideateShift = 6;
  constexpr std::uint32_t kImmideateMask = 0x7FF;
} // namespace utype


struct Instruction {
  std::uint32_t raw;
  std::uint8_t opcode;

  std::uint8_t rs1;
  std::uint8_t rs2;
  std::uint8_t rt1;
  std::uint8_t rt2;
  std::uint8_t rd;

  std::uint8_t base;
  std::uint32_t offset;

  std::uint32_t immediate;

  enum Type {
    RType,
    IType,
    SType,
    UType,
  } type;
};

namespace instructions_opcodes {
  const std::unordered_map<std::uint8_t, Instruction::Type> kInstructionsMap = {
    { 0b001101, Instruction::RType}, // NOR
    { 0b111100, Instruction::SType}, // LDP
    { 0b111001, Instruction::IType}, // CBIT
    { 0b110011, Instruction::RType}, // BDEP
    { 0b011010, Instruction::RType}, // ADD
    { 0b001100, Instruction::IType}, // SSAT
    { 0b110010, Instruction::SType}, // ST
    { 0b110101, Instruction::RType}, // CLZ
    { 0b000110, Instruction::SType}, // BNE
    { 0b001010, Instruction::SType}, // LD
    { 0b101001, Instruction::RType}, // XOR
    { 0b111000, Instruction::UType}, // SYSCALL
    { 0b011110, Instruction::SType}, // BEQ
    { 0b110110, Instruction::SType}, // J
  };
}

class InstructionParser {
 private:
  static constexpr std::uint8_t kStartOpcode = 0xFC;
  static constexpr std::uint8_t kEndOpcode = 0x3F;
  
  static constexpr std::uint8_t kFourBitMask = 0x0F;
  static constexpr std::uint8_t kFiveBitMask = 0x1F;

 public:
  static Instruction parse(std::uint32_t raw_instruction);

 private:
  static Instruction::Type determine_type(std::uint32_t instruction);
  
  static std::uint8_t get_opcode(std::uint32_t instruction);

  static void parse_rtype(Instruction& instruction);
  static void parse_itype(Instruction& instruction);
  static void parse_stype(Instruction& instruction);
  static void parse_utype(Instruction& instruction);

};

} // namespace simulator

#endif // INSTRUCTION_PARSER_HPP_
