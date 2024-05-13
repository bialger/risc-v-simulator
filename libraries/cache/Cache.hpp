#ifndef LIBRARIES_CACHE_CACHE_HPP_
#define LIBRARIES_CACHE_CACHE_HPP_

#include <vector>
#include <cstdint>

template<typename Policy>
class Cache {
 public:
  Cache() : policy_(), requests_count_(0), hits_count_(0) {}

  void ReadByte(size_t address) {
    Read(address, 1);
  }

  void ReadHalfWord(size_t address) {
    Read(address, 2);
  }

  void ReadWord(size_t address) {
    Read(address, 4);
  }

  void WriteByte(size_t address) {
    Write(address, 1);
  }

  void WriteHalfWord(size_t address) {
    Write(address, 2);
  }

  void WriteWord(size_t address) {
    Write(address, 4);
  }

  float GetHitRate() const {
    return static_cast<float>(hits_count_) / requests_count_;
  }

 private:
  Policy policy_;
  std::vector<std::vector<int>> cache;
  std::vector<std::vector<int>> is_line_changed;
  std::vector<std::vector<int>> bit_p;
  size_t requests_count_;
  size_t hits_count_;

  void Read(size_t address, size_t size) {
    // TODO: implement
  }

  void Write(size_t address, size_t size) {
    // TODO: implement
  }
};

#endif //LIBRARIES_CACHE_CACHE_HPP_
