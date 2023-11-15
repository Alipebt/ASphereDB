#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdint>
#include <string>
#include <vector>
#include "../include/options.h"
#include "../include/status.h"
class BlockHandle {
   public:
    uint64_t offset() { return offset_; };
    uint64_t size() { return size_; };
    void set_offset(uint64_t offset) { offset_ = offset; };
    void set_size(uint64_t size) { size_ = size; };

    void Encode(std::string *dst);
    STATUS Decode(const std::string &src);

   private:
    uint64_t offset_;
    uint64_t size_;
};

class BlockBuilder {
   public:
    BlockBuilder();
    ~BlockBuilder();

    void Add(const std::string &key, const std::string &value);
    void Reset();
    // 结束构造 Block，并返回Block
    std::string Finish();
    size_t BlockSize();

    // 返回 buffer 是否为空
    bool empty() { return buffer_.empty(); };

    // TEST
    void lookup();
    std::string getBuffer();

   private:
    const Options *op_;
    std::string buffer_;
    int counter_;
    std::string last_key_;
    std::vector<uint32_t> restarts_;
};

#endif