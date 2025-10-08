#ifndef INSTRUCTION_PARSER_HPP_
#define INSTRUCTION_PARSER_HPP_

#include <cstdint>

namespace simulator {
struct Instruction {
  std::uint32_t raw;
  std::uint8_t opcode;

  std::uint8_t rd;
  std::uint8_t rs1;
  std::uint8_t rs2;

  std::uint8_t funct3;
  std::uint8_t funct7;

  std::uint32_t immediate;

  enum Format {
    RType,
    IType,
    SType,
    UType,
  } format;
};

class InstructionParser {
 public:
  static Instruction parse(std::uint32_t instruction);

 private:
  static Instruction::Format determine_type(std::uint32_t instruction);
};

} // namespace simulator

#endif // INSTRUCTION_PARSER_HPP_
