#ifndef LIBRARIES_CACHE_BITPLRUPOLICY_HPP_
#define LIBRARIES_CACHE_BITPLRUPOLICY_HPP_

#include <vector>

#include "constants.hpp"
#include "common_functions.hpp"

class BitpLRUPolicy {
 public:
  BitpLRUPolicy() = delete;
  explicit BitpLRUPolicy(std::vector<std::vector<int32_t>>& cache);

  void UpdatePolicy(size_t index, size_t id);
  void AddLine(size_t index, int32_t tag);

 private:
  std::vector<std::vector<int32_t>>& cache_;
  std::vector<std::vector<bool>> has_cache_replacements_;
};

static_assert(CachePolicy<BitpLRUPolicy>);

#endif //LIBRARIES_CACHE_BITPLRUPOLICY_HPP_
