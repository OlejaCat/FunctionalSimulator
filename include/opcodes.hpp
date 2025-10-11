#ifndef OPCODES_HPP_
#define OPCODES_HPP_

#include <cstdint>

namespace simulator {

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

} // namespace simulator

#endif // OPCODES_HPP_
