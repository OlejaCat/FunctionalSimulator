#include <gtest/gtest.h>
#include "memory.hpp"
#include <stdexcept>
#include <vector>
#include <cstdint>

class MemoryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    memory_ = new simulator::Memory(1024); // 1KB памяти для тестов
  }

  void TearDown() override {
    delete memory_;
  }

  simulator::Memory* memory_;
};

// Тест инициализации памяти
TEST_F(MemoryTest, Initialization) {
  EXPECT_EQ(memory_->size(), 1024);
  
  // Проверяем, что память инициализирована нулями
  for (size_t i = 0; i < memory_->size(); ++i) {
    EXPECT_EQ(memory_->read_byte(i), 0);
  }
}

// Тест чтения/записи байта
TEST_F(MemoryTest, ByteReadWrite) {
  // Записываем и читаем в разных местах памяти
  memory_->write_byte(0, 0xAB);
  memory_->write_byte(100, 0xCD);
  memory_->write_byte(1023, 0xEF); // Последний байт

  EXPECT_EQ(memory_->read_byte(0), 0xAB);
  EXPECT_EQ(memory_->read_byte(100), 0xCD);
  EXPECT_EQ(memory_->read_byte(1023), 0xEF);
}

// Тест граничных случаев для байта
TEST_F(MemoryTest, ByteBoundary) {
  // Валидные адреса
  EXPECT_NO_THROW(memory_->write_byte(0, 0x11));
  EXPECT_NO_THROW(memory_->write_byte(1023, 0x22));
  
  // Невалидные адреса
  EXPECT_THROW(memory_->write_byte(1024, 0x33), std::range_error);
  EXPECT_THROW(memory_->read_byte(1024), std::range_error);
}

// Тест чтения/записи слова
TEST_F(MemoryTest, WordReadWrite) {
  memory_->write_word(0, 0x01234567);
  memory_->write_word(100, 0x89ABCDEF);
  memory_->write_word(1020, 0x13579BDF); // Последнее валидное слово

  EXPECT_EQ(memory_->read_word(0), 0x01234567);
  EXPECT_EQ(memory_->read_word(100), 0x89ABCDEF);
  EXPECT_EQ(memory_->read_word(1020), 0x13579BDF);
}

// Тест выравнивания для слов
TEST_F(MemoryTest, WordAlignment) {
  // Невыровненный адрес
  EXPECT_THROW(memory_->write_word(1, 0x12345678), std::runtime_error);
  EXPECT_THROW(memory_->read_word(3), std::runtime_error);
  
  // Валидные выровненные адреса
  EXPECT_NO_THROW(memory_->write_word(0, 0x11223344));
  EXPECT_NO_THROW(memory_->write_word(8, 0x55667788));
}

// Тест граничных случаев для слов
TEST_F(MemoryTest, WordBoundary) {
  // Валидные адреса
  EXPECT_NO_THROW(memory_->write_word(0, 0x12345678));
  EXPECT_NO_THROW(memory_->write_word(1020, 0x87654321));
  
  // Невалидные адреса (выход за границы)
  EXPECT_THROW(memory_->write_word(1021, 0x12345678), std::range_error);
  EXPECT_THROW(memory_->read_word(1024), std::range_error);
}

// Тест работы с блоками данных
TEST_F(MemoryTest, BlockReadWrite) {
  std::vector<uint8_t> write_block = {0x01, 0x02, 0x03, 0x04, 0x05};
  std::vector<uint8_t> read_block;
  
  // Записываем и читаем блок
  memory_->write_block(100, write_block);
  read_block = memory_->read_block(100, 5);
  
  EXPECT_EQ(read_block, write_block);
}

// Тест граничных случаев для блоков
TEST_F(MemoryTest, BlockBoundary) {
  std::vector<uint8_t> block = {0x01, 0x02, 0x03};
  
  // Валидный блок
  EXPECT_NO_THROW(memory_->write_block(0, block));
  EXPECT_NO_THROW(memory_->write_block(1021, {0x04, 0x05, 0x06}));
  
  // Блок слишком большой
  EXPECT_THROW(memory_->write_block(1022, {0x07, 0x08, 0x09}), std::range_error);
  EXPECT_THROW(memory_->read_block(1022, 3), std::range_error);
}

// Тест переполнения адреса
TEST_F(MemoryTest, AddressOverflow) {
  // Адрес near overflow с access_size=1
  EXPECT_NO_THROW(memory_->write_byte(1023, 0xFF));
  
  // Адрес с переполнением
  EXPECT_THROW(memory_->write_byte(UINT32_MAX, 0x11), std::range_error);
}

// Тест валидации адресов
TEST_F(MemoryTest, AddressValidation) {
  EXPECT_TRUE(memory_->is_valid_address(0));
  EXPECT_TRUE(memory_->is_valid_address(512));
  EXPECT_TRUE(memory_->is_valid_address(1023));
  
  EXPECT_FALSE(memory_->is_valid_address(1024));
  EXPECT_FALSE(memory_->is_valid_address(UINT32_MAX));
}

// Тест получения указателя на данные
TEST_F(MemoryTest, RawPointer) {
  uint8_t* ptr = memory_->get_row_pointer();
  const uint8_t* const_ptr = memory_->get_row_pointer();
  
  EXPECT_NE(ptr, nullptr);
  EXPECT_NE(const_ptr, nullptr);
  
  // Проверяем, что можем писать через указатель
  ptr[10] = 0xAA;
  EXPECT_EQ(memory_->read_byte(10), 0xAA);
  EXPECT_EQ(const_ptr[10], 0xAA);
}

// Тест порядка байт в слове (little-endian)
TEST_F(MemoryTest, Endianness) {
  memory_->write_word(0, 0x12345678);
  
  // Проверяем порядок байт (little-endian)
  EXPECT_EQ(memory_->read_byte(0), 0x78);
  EXPECT_EQ(memory_->read_byte(1), 0x56);
  EXPECT_EQ(memory_->read_byte(2), 0x34);
  EXPECT_EQ(memory_->read_byte(3), 0x12);
}

// Тест работы с нулевым размером памяти
//TEST(MemoryZeroSizeTest, ZeroSize) {
//  simulator::Memory zero_memory_(0);
//  
//  EXPECT_EQ(zero_memory_.size(), 0);
//  EXPECT_FALSE(zero_memory_.is_valid_address(0));
//  EXPECT_THROW(zero_memory_.read_byte(0), std::range_error);
//}
