#ifndef BIT_SHIFTS_HPP_
#define BIT_SHIFTS_HPP_

#include <cstdint>

namespace simulator {

namespace shifts {
  constexpr std::uint8_t  kOpcodeMask = 0x3F;
  constexpr std::uint32_t kOpcodeShift = 26;

  constexpr std::uint8_t  k5BitMask   = 0x1F;
  constexpr std::uint16_t k11BitMask  = 0x7FF;
  constexpr std::uint16_t k16BitMask  = 0xFFFF;
  constexpr std::uint32_t k20BitMask  = 0xFFFFF;
  constexpr std::uint32_t k26BitMask  = 0x3FFFFFF;

  constexpr std::uint32_t k6BitShift  = 6;
  constexpr std::uint32_t k11BitShift = 11;
  constexpr std::uint32_t k16BitShift = 16;
  constexpr std::uint32_t k21BitShift = 21;
} // namespace shifts

} // namespace simulator

#endif // BIT_SHIFTS_HPP_
