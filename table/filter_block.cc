#include "filter_block.h"
#include "../util/coding.h"

// 每个Filter Block的大小 （2K）
static const uint64_t kBlockSizelg = 11;
static const uint64_t kBlockSize = 1 << kBlockSizelg;

FilterBlockBuilder::FilterBlockBuilder(const Filter *filter)
    : filter_(filter) {}

void FilterBlockBuilder::Key2Bitmap() {
    if (keys_.size() == 0) {
        bitmap_offset_.push_back(bitmaps_.size());
        return;
    }
    filter_->CreateBitmap(keys_, bitmaps_);

    bitmap_offset_.push_back(bitmaps_.size());
    keys_.clear();
}

void FilterBlockBuilder::BuildBlocks(const uint64_t &block_size) {
    // 每2K大小构造一个bitmap
    uint64_t maps_num = (block_size / kBlockSize);
    while (maps_num > bitmap_offset_.size()) {
        Key2Bitmap();
    }
}

void FilterBlockBuilder::AddKey(const std::vector<std::string> &keys) {
    keys_.insert(keys_.end(), keys.begin(), keys.end());
}

void FilterBlockBuilder::AddKey(const std::string &key) {
    keys_.push_back(key);
}

std::string FilterBlockBuilder::Finish() {
    if (!keys_.empty()) {
        Key2Bitmap();
    };
    uint32_t bitmaps_size = bitmaps_.size();
    for (size_t i = 0; i < bitmap_offset_.size(); i++) {
        bitmaps_.append(reinterpret_cast<char *>(&bitmap_offset_[i]),
                        sizeof(int));
    }
    bitmaps_.append(reinterpret_cast<char *>(&bitmaps_size),
                    sizeof(bitmaps_size));
    bitmaps_.push_back(kBlockSizelg);
    return bitmaps_;
}

FilterBlockReader::FilterBlockReader(const Filter *filter,
                                     const std::string &filterBlock)
    : filter_(filter) {
    // log_ = filterBlock[n-1];
    const uint32_t bitmaps_size = Decode2Int(filterBlock.substr(
        filterBlock.size() - sizeof(char) - sizeof(int), sizeof(int)));
    bitmaps_ = filterBlock.substr(0, bitmaps_size);

    for (size_t i = bitmaps_size;
         i < filterBlock.size() - sizeof(char) - sizeof(int);
         i += sizeof(int)) {
        bitmap_offset_.push_back(
            Decode2Int(filterBlock.substr(i, sizeof(int))));
    }
}

bool FilterBlockReader::KeyMayMatch(const uint64_t &block_offset,
                                    const std::string &key) {
    int bitmap_index = block_offset >> kBlockSizelg;
    if (bitmap_index >= bitmap_offset_.size()) {
        // 错误，视为潜在的匹配项
        return true;
    }
    uint32_t bitmap_end = bitmap_offset_[bitmap_index];
    uint32_t bitmap_begin = 0;

    if (bitmap_index > 0) {
        if (bitmap_offset_[bitmap_index] == bitmap_offset_[bitmap_index - 1]) {
            return false;
        }
        bitmap_begin = bitmap_offset_[bitmap_index - 1];
    }

    std::string bitmap =
        bitmaps_.substr(bitmap_begin, bitmap_end - bitmap_begin);
    return filter_->KeyMayMatch(key, bitmap);
}
