#ifndef LIBRARIES_CACHE_LRUPOLICY_HPP_
#define LIBRARIES_CACHE_LRUPOLICY_HPP_

#include <vector>

#include "constants.hpp"
#include "common_functions.hpp"

class LRUPolicy {
 public:
  LRUPolicy() = delete;
  explicit LRUPolicy(std::vector<std::vector<int32_t>>& cache);

  void UpdatePolicy(size_t index, size_t id);
  void AddLine(size_t index, int32_t tag);

 private:
  std::vector<std::vector<int32_t>>& cache_;
};

static_assert(CachePolicy<LRUPolicy>);

#endif //LIBRARIES_CACHE_LRUPOLICY_HPP_
