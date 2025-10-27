#ifndef SYSCALLS_HPP_
#define SYSCALLS_HPP_


#include <cstdint>
namespace simulator::syscalls {

enum SyscallCommands {
  EXIT = 0,
};

constexpr std::uint8_t kNumberRegister = 8;
constexpr std::uint8_t kArgument0 = 0;
constexpr std::uint8_t kArgument1 = 1;
constexpr std::uint8_t kResult = 0;

} // namespace simulator::syscalls

#endif // SYSCALLS_HPP_
