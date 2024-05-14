#ifndef LIBRARIES_CACHE_CACHE_HPP_
#define LIBRARIES_CACHE_CACHE_HPP_

#include <vector>
#include <cstdint>

#include "constants.hpp"
#include "common_functions.hpp"

template<CachePolicy Policy>
class Cache {
 public:
  Cache() : cache_(CACHE_SETS), policy_(cache_), requests_count_(0), hits_count_(0) {}

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

  [[nodiscard]] float GetHitRate() const {
    return static_cast<float>(hits_count_) / requests_count_;
  }

 private:
  std::vector<std::vector<int32_t>> cache_;
  Policy policy_;
  size_t requests_count_;
  size_t hits_count_;

  void HandleRequest(size_t index, int32_t tag) {
    size_t id = Find<int32_t>(cache_[index], tag);

    if (id == cache_[index].size()) {
      policy_.AddLine(index, tag);
    } else {
      ++hits_count_;
      policy_.UpdatePolicy(index, id);
    }
  }

  void Read(size_t address, size_t size) {
    ++requests_count_;
    auto tag = static_cast<int32_t>(address >> (CACHE_INDEX_LEN + CACHE_OFFSET_LEN));
    size_t index = (address >> CACHE_OFFSET_LEN) & ((1 << CACHE_INDEX_LEN) - 1);

    HandleRequest(index, tag);
  }

  void Write(size_t address, size_t size) {
    ++requests_count_;
    auto tag = static_cast<int32_t>(address >> (CACHE_INDEX_LEN + CACHE_OFFSET_LEN));
    size_t index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN);

    HandleRequest(index, tag);
  }
};

#endif //LIBRARIES_CACHE_CACHE_HPP_
