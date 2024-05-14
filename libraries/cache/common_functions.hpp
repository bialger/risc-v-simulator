#ifndef LIBRARIES_ASM_RISCV_COMMON_FUNCTIONS_HPP_
#define LIBRARIES_ASM_RISCV_COMMON_FUNCTIONS_HPP_

#include <vector>
#include <cstdint>

template<typename T>
size_t Find(const std::vector<T>& array, T x)  {
  for (size_t i = 0; i < array.size(); ++i) {
    if (array[i] == x) {
      return i;
    }
  }

  return array.size();
}

template<typename Policy>
concept CachePolicy = requires(Policy policy, size_t index, size_t id, int32_t tag, std::vector<std::vector<int32_t>> cache) {
  policy.UpdatePolicy(index, id);
  policy.AddLine(index, tag);
  Policy(cache);
};

#endif //LIBRARIES_ASM_RISCV_COMMON_FUNCTIONS_HPP_
