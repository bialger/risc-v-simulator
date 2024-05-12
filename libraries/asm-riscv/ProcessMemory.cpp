#include "ProcessMemory.hpp"

void ProcessMemory::Write(size_t address, const std::vector<uint8_t>& data) {
  if (address + data.size() > memory.size()) {
    memory.resize(address + data.size());
  }

  for (size_t i = 0; i < data.size(); ++i) {
    memory[address + i] = data[i];
  }
}

std::vector<uint8_t> ProcessMemory::Read(size_t address, size_t size) {
  if (address + size > memory.size()) {
    return std::vector<uint8_t>(size);
  }

  std::vector<uint8_t> data(size);

  for (size_t i = 0; i < size; ++i) {
    data[i] = memory[address + i];
  }

  return data;
}

void ProcessMemory::WriteByte(size_t address, uint8_t data) {
  Write(address, std::vector<uint8_t>(1, data));
}

void ProcessMemory::WriteHalfWord(size_t address, uint16_t data) {
  std::vector<uint8_t> result(2);
  result[0] = data & 0xFF;
  result[1] = (data >> 8) & 0xFF;
  Write(address, result);
}

void ProcessMemory::WriteWord(size_t address, uint32_t data) {
  std::vector<uint8_t> result(4);
  result[0] = data & 0xFF;
  result[1] = (data >> 8) & 0xFF;
  result[2] = (data >> 16) & 0xFF;
  result[3] = (data >> 24) & 0xFF;
  Write(address, result);
}

uint8_t ProcessMemory::ReadByte(size_t address) {
  return Read(address, 1)[0];
}

uint16_t ProcessMemory::ReadHalfWord(size_t address) {
  std::vector<uint8_t> data = Read(address, 2);
  return (data[1] << 8) | data[0];
}

uint32_t ProcessMemory::ReadWord(size_t address) {
  std::vector<uint8_t> data = Read(address, 4);
  return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
}
