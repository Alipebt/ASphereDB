#include "block_builder.h"
#include <iostream>
#include "../util/coding.h"

BlockBuilder::BlockBuilder() : op_(new Options), counter_(0) {
    restarts_.push_back(0);
}

BlockBuilder::~BlockBuilder() { delete op_; }

void BlockBuilder::Add(const std::string &key, const std::string &value) {
    size_t shared_len = 0;
    if (counter_ < op_->block_restart_interval) {
        // 寻找key的相同内容长度
        const size_t min_len = std::min(key.size(), last_key_.size());
        while ((shared_len < min_len) &&
               (last_key_[shared_len] == key[shared_len])) {
            shared_len++;
        }
    } else {
        // 记录重启点
        restarts_.push_back(buffer_.size());
        counter_ = 0;
    }

    size_t non_shared_len = key.size() - shared_len;
    size_t value_len = value.size();

    // 以二进制追加
    buffer_.append(reinterpret_cast<char *>(&shared_len), sizeof(int));
    buffer_.append(reinterpret_cast<char *>(&non_shared_len), sizeof(int));
    buffer_.append(reinterpret_cast<char *>(&value_len), sizeof(int));

    buffer_.append(&key[shared_len]);
    buffer_.append(value);

    // last_key_.resize(shared);
    last_key_ =
        key.substr(0, key.size() - non_shared_len ? key.size() - non_shared_len
                                                  : key.size());
    counter_++;
    return;
}

void BlockBuilder::Reset() {
    buffer_.clear();
    counter_ = 0;
    last_key_.clear();
    restarts_.clear();
    restarts_.push_back(0);
    return;
}

std::string BlockBuilder::Finish() {
    for (size_t i = 0; i < restarts_.size(); i++) {
        buffer_.append(reinterpret_cast<char *>(&restarts_[i]), sizeof(int));
    }
    buffer_.append(reinterpret_cast<char *>(buffer_.size()), sizeof(int));
    return buffer_;
}

void BlockBuilder::lookup() {
    int writelen = 0;
    while (writelen < buffer_.size()) {
        int shared_head = writelen;
        int non_shared_len_head = writelen + sizeof(int);
        int value_len_head = writelen + 2 * sizeof(int);
        int key_head = writelen + 3 * sizeof(int);
        int value_head =
            writelen + 3 * sizeof(int) +
            Decode2Int(buffer_.substr(non_shared_len_head, sizeof(int)));

        int shared = Decode2Int(buffer_.substr(shared_head, sizeof(int)));
        int non_shared_len =
            Decode2Int(buffer_.substr(non_shared_len_head, sizeof(int)));
        int value_len = Decode2Int(buffer_.substr(value_len_head, sizeof(int)));

        std::string key = buffer_.substr(key_head, non_shared_len);
        std::string value = buffer_.substr(value_head, value_len);

        std::cout << "[" << shared << "]";
        std::cout << "[" << non_shared_len << "]";
        std::cout << "[" << value_len << "]";
        std::cout << "[" << key << "]";
        std::cout << "[" << value << "]" << std::endl;

        writelen = value_head + value_len;
    }
    return;
}

std::string BlockBuilder::getBuffer() { return buffer_; }
