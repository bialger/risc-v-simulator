#include "ProcessMemory.hpp"

ProcessMemory::ProcessMemory() : memory_(MEM_SIZE) {}

void ProcessMemory::Write(size_t address, const std::vector<uint8_t>& data) {
  if (address + data.size() > memory_.size()) {
    memory_.resize(address + data.size());
  }

  for (size_t i = 0; i < data.size(); ++i) {
    memory_[address + i] = data[i];
  }
}

std::vector<uint8_t> ProcessMemory::Read(size_t address, size_t size) const {
  if (address + size > memory_.size()) {
    return std::vector<uint8_t>(size);
  }

  std::vector<uint8_t> data(size);

  for (size_t i = 0; i < size; ++i) {
    data[i] = memory_[address + i];
  }

  return data;
}

void ProcessMemory::WriteByte(size_t address, uint8_t data) {
  Write(address, std::vector<uint8_t>(1, data));
  lru_cache_.WriteByte(address);
  bitp_cache_.WriteByte(address);
}

void ProcessMemory::WriteHalfWord(size_t address, uint16_t data) {
  std::vector<uint8_t> result(2);
  result[0] = data & 0xFF;
  result[1] = (data >> 8) & 0xFF;
  Write(address, result);
  lru_cache_.WriteHalfWord(address);
  bitp_cache_.WriteHalfWord(address);
}

void ProcessMemory::WriteWord(size_t address, uint32_t data) {
  std::vector<uint8_t> result(4);
  result[0] = data & 0xFF;
  result[1] = (data >> 8) & 0xFF;
  result[2] = (data >> 16) & 0xFF;
  result[3] = (data >> 24) & 0xFF;
  Write(address, result);
  lru_cache_.WriteWord(address);
  bitp_cache_.WriteWord(address);
}

uint8_t ProcessMemory::ReadByte(size_t address) {
  lru_cache_.ReadByte(address);
  bitp_cache_.ReadByte(address);
  return Read(address, 1)[0];
}

uint16_t ProcessMemory::ReadHalfWord(size_t address) {
  lru_cache_.ReadHalfWord(address);
  bitp_cache_.ReadHalfWord(address);
  std::vector<uint8_t> data = Read(address, 2);
  return (data[1] << 8) | data[0];
}

uint32_t ProcessMemory::ReadWord(size_t address) {
  lru_cache_.ReadWord(address);
  bitp_cache_.ReadWord(address);
  std::vector<uint8_t> data = Read(address, 4);
  return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
}

float ProcessMemory::GetLRUHitRate() const {
  return lru_cache_.GetHitRate();
}

float ProcessMemory::GetBitpLRUHitRate() const {
  return bitp_cache_.GetHitRate();
}
