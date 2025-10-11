#ifndef INSTRUCTION_FORMATS_HPP_
#define INSTRUCTION_FORMATS_HPP_

#include <cstdint>
#include <variant>

namespace simulator {

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

} // namespace simulator


#endif // INSTRUCTION_FORMATS_HPP_
