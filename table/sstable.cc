#include "sstable.h"
#include "../util/bloom.h"
using namespace Table;

SSTableBuilder::SSTableBuilder(const std::string &dbname, FileWriter *writer,
                               const Options &op)
    : dbname_(dbname),
      writer_(writer),
      op_(op),
      filter_block_(new FilterBlockBuilder(op.filter)),
      offset_(0) {}

Table::SSTableBuilder::~SSTableBuilder() { delete filter_block_; }

void SSTableBuilder::Add(const std::string &key, const std::string &value) {
    if (filter_block_ != nullptr) {
        filter_block_->AddKey(key);
    }
    data_block_.Add(key, value);
    last_key_ = key;
    if (data_block_.BlockSize() >= op_.block_size) {
        FlushData();
    }
}

STATUS SSTableBuilder::WriteBlock(const std::string &data,
                                  BlockHandle &handle) {
    buffer_ = data;
    handle.set_offset(offset_);
    handle.set_offset(buffer_.size());
    STATUS st = writer_->Append(buffer_);
    if (st != S_OK) {
        return st;
    }
    offset_ += buffer_.size();
    return S_OK;
}

STATUS SSTableBuilder::FlushData() {
    if (data_block_.empty()) {
        return S_OK;
    }
    WriteBlock(data_block_.Finish(), data_block_handle_);

    data_block_.Reset();
    writer_->FlushBuf();
    filter_block_->BuildBlocks(offset_);
    std::string handle;
    data_block_handle_.Encode(&handle);
    index_block_.Add(last_key_, handle);
    return S_OK;
}

STATUS SSTableBuilder::Finish() {
    if (!data_block_.empty()) {
        FlushData();
    }
    std::string handle;
    // Filter Block
    WriteBlock(filter_block_->Finish(), filter_block_handle_);
    // Meta Index Block
    std::string key = "filter.";
    key.append(op_.filter->FilterName());
    filter_block_handle_.Encode(&handle);
    meta_block_.Add(key, handle);
    handle.clear();
    WriteBlock(meta_block_.Finish(), meta_block_handle_);
    // Index Block
    index_block_handle_.Encode(&handle);
    index_block_.Add(last_key_, handle);
    handle.clear();
    WriteBlock(index_block_.Finish(), index_block_handle_);
    // Footer Block
    footer_block_.Build(meta_block_handle_, index_block_handle_);
    footer_block_.Encode(&handle);
    writer_->Append(handle);
    offset_ += handle.size();
    handle.clear();
    return S_OK;
}