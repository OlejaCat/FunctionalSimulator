#include <gtest/gtest.h>
#include "instruction_parser.hpp"

TEST(InstructionParserTest, NOR_RTypeInstruction) {
  // Binary: 000000 00001 00010 00011 00000 001101
  // Hex: 0x0022180D
  std::uint32_t raw_instruction = 0x0022180D;

  // Act
  simulator::Instruction instruction = simulator::InstructionParser::parse(raw_instruction);

  // Assert
  EXPECT_EQ(instruction.opcode, 0b001101);
  EXPECT_EQ(instruction.type, simulator::Instruction::RType);
  EXPECT_EQ(instruction.rs1, 1);
  EXPECT_EQ(instruction.rt1, 2);
  EXPECT_EQ(instruction.rd, 3);
}

TEST(InstructionParserTest, ADD_RTypeInstruction) {
  // Arrange
  // Binary: 000000 00100 00101 00110 00000 011010
  // Hex: 0x0085301A
  std::uint32_t raw_instruction = 0x0085301A;

  // Act
  simulator::Instruction instruction = simulator::InstructionParser::parse(raw_instruction);

  // Assert
  EXPECT_EQ(instruction.opcode, 0b011010);
  EXPECT_EQ(instruction.type, simulator::Instruction::RType);
  EXPECT_EQ(instruction.rs1, 4);
  EXPECT_EQ(instruction.rt1, 5);
  EXPECT_EQ(instruction.rd, 6);
}

TEST(InstructionParserTest, LDP_STypeInstruction) {
  // Arrange
  // Binary: 111100 00001 00010 00011 00100100011
  // Hex: 0xF0221923
  std::uint32_t raw_instruction = 0xF0221923;

  // Act
  simulator::Instruction instruction = simulator::InstructionParser::parse(raw_instruction);

  // Assert
  EXPECT_EQ(instruction.opcode, 0b111100);
  EXPECT_EQ(instruction.type, simulator::Instruction::SType);
  EXPECT_EQ(instruction.base, 1);
  EXPECT_EQ(instruction.rt1, 2);
  EXPECT_EQ(instruction.rt2, 3);
  EXPECT_EQ(instruction.offset, 0x123);
}

TEST(InstructionParserTest, LD_STypeInstruction) {
  // Binary: 001010 01110 01000 0000 0000 0011 1111
  // Let's assume offset is in bits 10:0 = 0x3F
  // Hex: 0x29C8003F
  std::uint32_t raw_instruction = 0x29C8003F;

  // Act
  simulator::Instruction instruction = simulator::InstructionParser::parse(raw_instruction);

  // Assert
  EXPECT_EQ(instruction.opcode, 0b001010);
  EXPECT_EQ(instruction.type, simulator::Instruction::SType);
  EXPECT_EQ(instruction.base, 14);
  EXPECT_EQ(instruction.rt1, 8);
  EXPECT_EQ(instruction.offset, 0x3F);
}

