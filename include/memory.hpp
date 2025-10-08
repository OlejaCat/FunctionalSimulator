#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <cstdint>
#include <vector>

namespace simulator {

class Memory {
 public:
  Memory(std::size_t memory_size);

  std::uint8_t* get_row_pointer();
  const std::uint8_t* get_row_pointer() const;

  std::uint8_t read_byte(std::uint32_t address) const;

  void write_byte(std::uint32_t address, std::uint8_t byte);

 private:
  std::vector<std::uint8_t> data_;
};

}

#endif // MEMORY_HPP_
