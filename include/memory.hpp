#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <cstdint>
#include <vector>

namespace simulator {

class Memory {
 private:
  static constexpr std::size_t kBitInByte = 8;
  static constexpr std::uint32_t kByteMask = 0xFF;

  static constexpr std::size_t kByteAccessSize = 1;
  static constexpr std::size_t kWordAccessSize = 4;

 public:
  Memory(std::size_t memory_size);

  std::uint8_t read_byte(std::uint32_t address) const;
  void write_byte(std::uint32_t address, std::uint8_t byte);

  std::uint32_t read_word(std::uint32_t address) const;
  void write_word(std::uint32_t address, std::uint32_t word);

  std::vector<std::uint8_t> read_block(std::uint32_t address,
                                       std::size_t size) const;
  void write_block(std::uint32_t address,
                   const std::vector<std::uint8_t>& block);

  std::size_t size() const;
  bool is_valid_address(std::uint32_t address) const;

  std::uint8_t* get_row_pointer();
  const std::uint8_t* get_row_pointer() const;

 private:
  static void check_allignment(std::uint32_t address, std::size_t allignment);
  void check_address_range(std::uint32_t address,
                           std::size_t access_size) const;

  std::size_t memory_size_;
  std::vector<std::uint8_t> data_;
};

}  // namespace simulator

#endif // MEMORY_HPP_
