#ifndef INSTRUCTION_PARSER_HPP_
#define INSTRUCTION_PARSER_HPP_

#include <cstdint>
#include "instruction_formats.hpp"


namespace simulator {

class InstructionParser {
 public:
    static Instruction parse(std::uint32_t raw_instruction);

    static std::uint8_t get_opcode(std::uint32_t instruction);
};

} // namespace simulator

#endif // INSTRUCTION_PARSER_HPP_
