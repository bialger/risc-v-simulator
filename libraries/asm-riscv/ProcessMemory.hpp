#ifndef LIBRARIES_ASM_RISCV_PROCESSMEMORY_HPP_
#define LIBRARIES_ASM_RISCV_PROCESSMEMORY_HPP_

#include <cstdint>
#include <vector>

#include "../cache/Cache.hpp"
#include "../cache/LRUPolicy.hpp"
#include "../cache/BitpLRUPolicy.hpp"

class ProcessMemory {
 public:
  ProcessMemory();

  void WriteByte(size_t address, uint8_t data);
  void WriteHalfWord(size_t address, uint16_t data);
  void WriteWord(size_t address, uint32_t data);

  [[nodiscard]] uint8_t ReadByte(size_t address);
  [[nodiscard]] uint16_t ReadHalfWord(size_t address);
  [[nodiscard]] uint32_t ReadWord(size_t address);

  [[nodiscard]] float GetLRUHitRate() const;
  [[nodiscard]] float GetBitpLRUHitRate() const;

 private:
  std::vector<uint8_t> memory_;
  Cache<LRUPolicy> lru_cache_;
  Cache<BitpLRUPolicy> bitp_cache_;

  void Write(size_t address, const std::vector<uint8_t>& data);
  [[nodiscard]] std::vector<uint8_t> Read(size_t address, size_t size) const;
};

#endif //LIBRARIES_ASM_RISCV_PROCESSMEMORY_HPP_
