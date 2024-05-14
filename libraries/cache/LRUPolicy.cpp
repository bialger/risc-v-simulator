#include "LRUPolicy.hpp"

LRUPolicy::LRUPolicy(std::vector<std::vector<int32_t>>& cache) : cache_(cache) {}

void LRUPolicy::UpdatePolicy(size_t index, size_t id) {
  int32_t tmp_tag = cache_[index][id];
  cache_[index].erase(cache_[index].begin() + static_cast<int64_t>(id));
  cache_[index].push_back(tmp_tag);
}

void LRUPolicy::AddLine(size_t index, int32_t tag) {
  if (cache_[index].size() == CACHE_WAY) {
    cache_[index].erase(cache_[index].begin());
  }

  cache_[index].push_back(tag);
}
