#ifndef LIBRARIES_CACHE_CONSTANTS_HPP_
#define LIBRARIES_CACHE_CONSTANTS_HPP_

#include <cstdint>

constexpr size_t MEM_SIZE = 524288;
constexpr size_t ADDR_LEN = 19; // log2(MEM_SIZE)
constexpr size_t CACHE_WAY = 4; // CACHE_LINE_COUNT / (2 ^ CACHE_INDEX_LEN)
constexpr size_t CACHE_TAG_LEN = 10; // ADDR_LEN - CACHE_INDEX_LEN - CACHE_OFFSET_LEN;
constexpr size_t CACHE_INDEX_LEN = 4;
constexpr size_t CACHE_OFFSET_LEN = 5;
constexpr size_t CACHE_SIZE = 2048;
constexpr size_t CACHE_LINE_SIZE = 32; // 2 ^ CACHE_OFFSET_LEN
constexpr size_t CACHE_LINE_COUNT = 64; // CACHE_SIZE / CACHE_LINE_SIZE
constexpr size_t CACHE_SETS = 16; // (2 ^ CACHE_INDEX_LEN)

#endif //LIBRARIES_CACHE_CONSTANTS_HPP_
