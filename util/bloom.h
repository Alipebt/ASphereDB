#ifndef BLOOM_H_
#define BLOOM_H_
#include "../include/filter.h"
class BloomFilter : public Filter {
   public:
    BloomFilter() = delete;
    BloomFilter(int bits_per_key);

    std::string FilterName() const override;
    // 将新数据追加到 bitmaps_ 中
    void BuildBitmap(const std::vector<std::string> &keys,
                      std::string &bitmap) const override;
    bool KeyMayMatch(const std::string &key,
                     const std::string &bitmap) const override;

   private:
    size_t bits_per_key_;
    size_t hash_count_;
};
#endif