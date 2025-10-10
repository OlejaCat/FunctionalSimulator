#include "memory.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>


namespace simulator {

Memory::Memory(std::size_t memory_size)
    : memory_size_(memory_size), data_(memory_size, 0) {}

std::uint8_t Memory::read_byte(std::uint32_t address) const {
  check_address_range(address, kByteAccessSize);
  return data_[address];
}

void Memory::write_byte(std::uint32_t address, std::uint8_t byte) {
  check_address_range(address, kByteAccessSize);
  data_[address] = byte;
}

std::uint32_t Memory::read_word(std::uint32_t address) const {
  check_address_range(address, kWordAccessSize);
  check_allignment(address, kWordAccessSize);

  std::uint32_t value = 0;
  for (std::size_t i = 0; i < kWordAccessSize; ++i) {
    value |= static_cast<std::uint32_t>(data_[address + i]) << (kBitInByte * i);
  }

  return value;
}

void Memory::write_word(std::uint32_t address, std::uint32_t word) {
  check_address_range(address, kWordAccessSize);
  check_allignment(address, kWordAccessSize);

  for (std::size_t i = 0; i < kWordAccessSize; ++i) {
    data_[address + i] =
        static_cast<std::uint8_t>((word >> (kBitInByte * i)) & kByteMask);
  }
}

std::vector<std::uint8_t> Memory::read_block(
    std::uint32_t address, std::size_t size) const {
  check_address_range(address, size);

  std::vector<std::uint8_t> block(size);
  std::copy(data_.begin() + address, data_.begin() + address + size,
            block.begin());

  return block;
}

void Memory::write_block(std::uint32_t address,
                                    const std::vector<std::uint8_t>& block) {
  check_address_range(address, block.size());

  std::copy(block.begin(), block.end(), data_.begin() + address);
}

std::size_t Memory::size() const {
  return memory_size_;
}

bool Memory::is_valid_address(std::uint32_t address) const {
  return address < memory_size_;
}

std::uint8_t* simulator::Memory::get_row_pointer() {
  return data_.data();
}

const std::uint8_t* simulator::Memory::get_row_pointer() const {
  return data_.data();
}

void Memory::check_allignment(std::uint32_t address,
                                         std::size_t allignment) {
  if (address % allignment != 0) {
    throw std::runtime_error("Unaligned memory access at address: "
                             + std::to_string(address));
  }
}

void Memory::check_address_range(std::uint32_t address,
                                            std::size_t access_size) const {
  if (address + access_size > memory_size_
      || address + access_size < address) {
    throw std::range_error("Memory access out of range: address="
                           + std::to_string(address)
                           + ", size=" + std::to_string(access_size));
  }
}

} // namespace simulator

