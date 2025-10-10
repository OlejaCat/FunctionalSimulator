#ifndef INSTRUCTION_PARSER_HPP_
#define INSTRUCTION_PARSER_HPP_

#include <cstdint>
#include <variant>

namespace simulator {

namespace shifts {
  constexpr std::uint8_t  k5BitMask   = 0x1F;
  constexpr std::uint16_t k11BitMask  = 0x7FF;
  constexpr std::uint16_t k16BitMask  = 0xFFFF;
  constexpr std::uint32_t k20BitMask  = 0xFFFFF;
  constexpr std::uint32_t k26BitMask  = 0x3FFFFFF;

  constexpr std::uint32_t k6BitShift  = 6;
  constexpr std::uint32_t k11BitShift = 11;
  constexpr std::uint32_t k16BitShift = 16;
  constexpr std::uint32_t k21BitShift = 21;
}

namespace opcodes {
    constexpr std::uint8_t kNOR     = 0b001101;
    constexpr std::uint8_t kLDP     = 0b111100;
    constexpr std::uint8_t kCBIT    = 0b111001;
    constexpr std::uint8_t kBDEP    = 0b110011;
    constexpr std::uint8_t kADD     = 0b011010;
    constexpr std::uint8_t kSSAT    = 0b001100;
    constexpr std::uint8_t kST      = 0b110010;
    constexpr std::uint8_t kCLZ     = 0b110101;
    constexpr std::uint8_t kBNE     = 0b000110;
    constexpr std::uint8_t kLD      = 0b001010;
    constexpr std::uint8_t kXOR     = 0b101001;
    constexpr std::uint8_t kSYSCALL = 0b111000;
    constexpr std::uint8_t kBEQ     = 0b011110;
    constexpr std::uint8_t kJj      = 0b110110;
} // namespace opcodes


struct RFormat {
    std::uint8_t rd, rs, rt;
};

struct BdepFormat {
    std::uint8_t rd, rs1, rs2;
};

struct ClzFormat {
    std::uint8_t rd, rs;
};

struct RdRsImm5Format {
    std::uint8_t rd, rs, imm5;
};

struct MemBaseRtOffset16Format {
    std::uint8_t base, rt;
    std::uint16_t offset;
};

struct BranchRsRtOffset16Format {
    std::uint8_t rs, rt;
    std::uint16_t offset;
};

struct LdpFormat {
    std::uint8_t rt1, rt2, base;
    std::uint16_t offset;
};

struct JTarget26Format {
    std::uint32_t target_index;
};

struct SyscallFormat {
    std::uint32_t code;
};

struct Instruction {
    std::uint32_t raw;
    std::uint8_t opcode;

    std::variant<
        RFormat,
        BdepFormat,
        ClzFormat,
        RdRsImm5Format,
        MemBaseRtOffset16Format,
        BranchRsRtOffset16Format,
        LdpFormat,
        JTarget26Format,
        SyscallFormat
    > fields;
};


class InstructionParser {
 private:
    static constexpr std::uint8_t kOpcodeMask = 0x3F; 
    static constexpr std::uint32_t kOpcodeShift = 26;

 public:
    static Instruction parse(std::uint32_t raw_instruction);

    static std::uint8_t get_opcode(std::uint32_t instruction);
};

} // namespace simulator

#endif // INSTRUCTION_PARSER_HPP_
