#include <gtest/gtest.h>
#include <memory>
#include "cpu.hpp"
#include "memory.hpp"
#include "opcodes.hpp"


class CpuRFormatTest : public ::testing::Test {
 protected:
  simulator::Memory memory_ {1024}; 
  std::unique_ptr<simulator::Cpu> cpu_;

  void SetUp() override {
    cpu_ = std::make_unique<simulator::Cpu>(memory_);
    cpu_->set_pc(0);

    cpu_->set_register(1, 0x000000FF);
    cpu_->set_register(2, 0x00000001);
  }

  static std::uint32_t create_rformat(std::uint8_t opcode, std::uint8_t rs, std::uint8_t rt, std::uint8_t rd) {
    return (static_cast<std::uint32_t>(rs) << 21) | 
           (static_cast<std::uint32_t>(rt) << 16) | 
           (static_cast<std::uint32_t>(rd) << 11) | 
           (static_cast<std::uint32_t>(opcode));
  }

  void run_rformat(std::uint32_t raw_instruction) {
    memory_.write_word(cpu_->get_pc(), raw_instruction);
    cpu_->pipeline_cycle();
    
    ASSERT_EQ(cpu_->get_pc(), 4);
  }
};

class CpuMemoryFormatTest : public ::testing::Test {
 protected:
  simulator::Memory memory_ {1024};
  std::unique_ptr<simulator::Cpu> cpu_;

  static constexpr std::uint32_t kBaseAddress = 0x00F0;

  void SetUp() override {
    cpu_ = std::make_unique<simulator::Cpu>(memory_);
    cpu_->set_pc(0);

    cpu_->set_register(2, kBaseAddress);
  }

  std::uint32_t create_memory_format(std::uint8_t opcode, std::uint8_t base, std::uint8_t rt, std::uint16_t offset) {
    return (static_cast<std::uint32_t>(opcode) << 26) | 
           (static_cast<std::uint32_t>(base) << 21) | 
           (static_cast<std::uint32_t>(rt) << 16) | 
           (static_cast<std::uint32_t>(offset));
  }

  void run_memory_format(std::uint32_t raw_instruction) {
    memory_.write_word(cpu_->get_pc(), raw_instruction);
    cpu_->pipeline_cycle();

    ASSERT_EQ(cpu_->get_pc(), 4);
  }
};


TEST_F(CpuRFormatTest, ADD_Basic) {
  std::uint32_t expexted = 0x00000100;  

  std::uint32_t raw_instruction = create_rformat(simulator::opcodes::kADD, 1, 2, 3);
  run_rformat(raw_instruction);
  
  ASSERT_EQ(cpu_->get_register(3), expexted);
}

TEST_F(CpuRFormatTest, XOR_Basic) {
  std::uint32_t expexted = 0x000000FE; 

  std::uint32_t raw_instruction = create_rformat(simulator::opcodes::kXOR, 1, 2, 3);
  run_rformat(raw_instruction);
  
  ASSERT_EQ(cpu_->get_register(3), expexted);
}

TEST_F(CpuRFormatTest, NOR_Basic) {
  std::uint32_t expexted = 0xFFFFFF00; 

  std::uint32_t raw_instruction = create_rformat(simulator::opcodes::kNOR, 1, 2, 3);
  run_rformat(raw_instruction);
  
  ASSERT_EQ(cpu_->get_register(3), expexted);
}

TEST_F(CpuMemoryFormatTest, LD_PositiveOffset) {
  constexpr std::uint16_t kOffset = 0x0100; 
  constexpr std::uint32_t kExpectedData = 0x00C0FFEE;
  constexpr std::uint32_t kTargetAddress = kBaseAddress + kOffset;

  memory_.write_word(kTargetAddress, kExpectedData);
  cpu_->set_register(1, 0x0); 

  std::uint32_t raw_inst = create_memory_format(simulator::opcodes::kLD, 2, 1, kOffset);
  run_memory_format(raw_inst);

  ASSERT_EQ(cpu_->get_register(1), kExpectedData)
    << "LD failed to load data with positive offset.";
}
