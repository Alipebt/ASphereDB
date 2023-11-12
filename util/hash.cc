#include "hash.h"

uint32_t Hash(const std::string &str, const uint32_t &seed) {
    // murmur3 hash
    const uint32_t mu1 = 0xcc9e2d51;
    const uint32_t mu2 = 0x1b873593;

    const uint32_t r1 = 15;
    const uint32_t r2 = 13;

    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    uint32_t h = seed;

    const uint32_t len = str.size();
    for (uint32_t i = 0; i <= len - 4; i += 4) {
        // 4字节转置
        uint32_t k = ((uint32_t)str.at(i) << 24) |
                     ((uint32_t)str.at(i + 1) << 16) |
                     ((uint32_t)str.at(i + 2) << 8) | (uint32_t)str.at(i + 3);

        // ROT r1
        k *= mu1;
        k = (k << r1) | (k >> (32 - r1));
        k *= mu2;

        // ROT r2
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    uint32_t k = 0;
    switch (len % 4) {
        case 3:
            k ^= ((uint32_t)str.at(len - 1) << 16);
        case 2:
            k ^= ((uint32_t)str.at(len - 2) << 8);
        case 1:
            k ^= (uint32_t)str.at(len - 3);
        default:
            break;
    }

    k *= mu1;
    k = (k << r1) | (k >> (32 - r1));
    k *= mu2;
    h ^= k;
    h ^= len;

    // 经验
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}
