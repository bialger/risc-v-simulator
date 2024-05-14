#include "BitpLRUPolicy.hpp"

BitpLRUPolicy::BitpLRUPolicy(std::vector<std::vector<int>>& cache)
    : cache_(cache), has_cache_replacements_(CACHE_SETS, std::vector<bool>(CACHE_WAY, false)) {
  for (auto& line : cache_) {
    line.resize(CACHE_WAY);

    for (int32_t& word : line) {
      word = -1;
    }
  }
}

void BitpLRUPolicy::UpdatePolicy(size_t index, size_t id) {
  has_cache_replacements_[index][id] = true;

  for (size_t i = 0; i < has_cache_replacements_[index].size(); ++i) {
    if (!has_cache_replacements_[index][i]) {
      return;
    }
  }

  for (size_t i = 0; i < has_cache_replacements_[index].size(); ++i) {
    if (i != id) {
      has_cache_replacements_[index][i] = false;
    }
  }
}

void BitpLRUPolicy::AddLine(size_t index, int32_t tag) {
  size_t id = Find(has_cache_replacements_[index], false);
  cache_[index][id] = tag;
  UpdatePolicy(index, id);
}
