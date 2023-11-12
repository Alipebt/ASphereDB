#include <gtest/gtest.h>
#include "../util/bloom.h"
#include "../util/hash.h"

/*

TEST(util_test, Hash) {
    BloomFilter bloom(3);
    std::vector<std::string> keys;
    for (int i = 0; i < 1000; i++) {
        keys.push_back("key" + std::to_string(i));
    }
    std::string bitmap;
    bloom.CreateBitmap(keys, bitmap);
    for (int i = 1000; i < 2000; i++) {
        std::cout << bloom.KeyMayMatch("key" + std::to_string(i), bitmap);
    }
    std::cout << std::endl;
}
*/