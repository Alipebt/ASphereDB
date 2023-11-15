#include "bloom.h"
#include <stdint.h>
#include "../util/hash.h"

BloomFilter::BloomFilter(int bits_per_key) : bits_per_key_(bits_per_key) {
    // 哈希计算次数的最优解: k=n/m*ln2，ln2 =~ 0.69
    hash_count_ = static_cast<size_t>(bits_per_key_ * 0.69);
    if (hash_count_ < 1) {
        hash_count_ = 1;
    } else if (hash_count_ > 30) {
        hash_count_ = 30;
    }
};

std::string BloomFilter::FilterName() const { return "Bloom"; }

void BloomFilter::BuildBitmap(const std::vector<std::string> &keys,
                               std::string &bitmap) const {
    size_t bitmap_size = keys.size() * bits_per_key_;
    size_t init_size = bitmap.size();

    // bitmap太小会导致误判率提高
    if (bitmap_size < 64) {
        bitmap_size = 64;
    }

    // 字节对齐
    size_t bytes = (bitmap_size + 7) / 8;
    bitmap_size = bytes * 8;

    bitmap.resize(init_size + bytes, 0);
    auto result = &bitmap[init_size];

    // 双哈希模拟多哈希
    for (const auto &key : keys) {
        uint32_t h1 = Hash(key, 0x4edfaa77);
        uint32_t h2 = Hash(key, 0x1bc902bf);
        for (size_t i = 0; i < hash_count_; ++i) {
            uint32_t p = (h1 + i * h2) % bitmap_size;
            result[p / 8] |= (1 << (p % 8));
        }
    }
}

bool BloomFilter::KeyMayMatch(const std::string &key,
                              const std::string &bitmap) const {
    uint32_t h1 = Hash(key, 0x4edfaa77);
    uint32_t h2 = Hash(key, 0x1bc902bf);
    for (size_t i = 0; i < hash_count_; ++i) {
        uint32_t p = (h1 + i * h2) % (bitmap.size() * 8);
        if ((bitmap[p / 8] & (1 << (p % 8))) == 0) {
            return false;
        }
    }
    return true;
}
