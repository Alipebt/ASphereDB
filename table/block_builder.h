#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdint>
#include <string>
#include <vector>
#include "../include/options.h"
#include "../include/status.h"

class BlockBuilder {
   public:
    BlockBuilder();
    ~BlockBuilder();

    STATUS Add(const std::string &key, const std::string &value);
    STATUS Reset();
    STATUS Finish();

    // TEST
    STATUS Lookup();

   private:
    const Options *op_;
    std::string buffer_;
    int counter_;
    std::string last_key_;
    std::vector<uint32_t> restarts_;
};

#endif