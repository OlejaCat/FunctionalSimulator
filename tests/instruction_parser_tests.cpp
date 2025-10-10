#include <gtest/gtest.h>
#include "instruction_parser.hpp"

TEST(InstructionParserTest, NOR_RTypeInstruction) {
  // Binary: 000000 00001 00010 00011 00000 001101
  // Hex: 0x0022180D
  constexpr std::uint32_t kRawInstruction = 0x0022180D;

  simulator::Instruction instruction = simulator::InstructionParser::parse(kRawInstruction);
  const auto& r_format = get<simulator::RFormat>(instruction.fields);

  EXPECT_EQ(instruction.opcode, 0b001101);
  EXPECT_EQ(r_format.rs, 1);
  EXPECT_EQ(r_format.rt, 2);
  EXPECT_EQ(r_format.rd, 3);
}

TEST(InstructionParserTest, ADD_RTypeInstruction) {
  // Binary: 000000 00100 00101 00110 00000 011010
  // Hex: 0x0085301A
  constexpr std::uint32_t kRawInstruction = 0x0085301A;

  simulator::Instruction instruction = simulator::InstructionParser::parse(kRawInstruction);
  const auto& r_format = get<simulator::RFormat>(instruction.fields);

  EXPECT_EQ(instruction.opcode, 0b011010);
  EXPECT_EQ(r_format.rs, 4);
  EXPECT_EQ(r_format.rt, 5);
  EXPECT_EQ(r_format.rd, 6);
}

TEST(InstructionParserTest, LDP_STypeInstruction) {
  // Binary: 111100 00001 00010 00011 00100100011
  // Hex: 0xF0221923
  constexpr std::uint32_t kRawInstruction = 0xF0221923;

  simulator::Instruction instruction = simulator::InstructionParser::parse(kRawInstruction);
  const auto& ldp_format = get<simulator::LdpFormat>(instruction.fields);

  EXPECT_EQ(instruction.opcode, 0b111100);
  EXPECT_EQ(ldp_format.base, 1);
  EXPECT_EQ(ldp_format.rt1, 2);
  EXPECT_EQ(ldp_format.rt2, 3);
  EXPECT_EQ(ldp_format.offset, 0x123);
}

