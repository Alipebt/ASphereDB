#ifndef FILTER_BLOCK_H_
#define FILTER_BLOCK_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "../include/filter.h"

class FilterBlockBuilder {
   public:
    FilterBlockBuilder() = delete;
    FilterBlockBuilder(const Filter *filter);

    // 构造 Filter Block
    void BuildBlocks(const uint64_t &block_size);
    void AddKey(const std::vector<std::string> &keys);
    void AddKey(const std::string &key);
    // 结束构造，返回Filter Block
    std::string Finish();

   private:
    void Key2Bitmap();

    const Filter *filter_;
    std::vector<std::string> keys_;
    std::vector<uint32_t> bitmap_offset_;  // 每个bitmap的偏移量
    std::string bitmaps_;
};

class FilterBlockReader {
   public:
    FilterBlockReader() = delete;
    FilterBlockReader(const Filter *filter, const std::string &filterBlock);

    bool KeyMayMatch(const uint64_t &bitmap_offset, const std::string &key);

   private:
    const Filter *filter_;
    std::string bitmaps_;
    std::vector<uint32_t> bitmap_offset_;
};

#endif