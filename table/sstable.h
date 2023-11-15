#ifndef SSTABLE_H_
#define SSTABLE_H_
#include <string>
#include "../include/options.h"
#include "../include/status.h"
#include "./util/file_manager.h"
#include "block.h"
#include "filter_block.h"
#include "footer_block.h"
namespace Table {
class SSTableBuilder {
   public:
    SSTableBuilder() = delete;
    SSTableBuilder(const std::string &dbname, FileWriter *writer,
                   const Options &op);
    ~SSTableBuilder();

    void Add(const std::string &key, const std::string &value);
    // 将数据刷入磁盘
    STATUS FlushData();
    // 写入块
    STATUS WriteBlock(const std::string &data, BlockHandle &handle);
    // 形成完整的SSTable文件
    STATUS Finish();

   private:
    std::string dbname_;
    FileWriter *writer_;
    Options op_;
    std::string last_key_;

    // data block
    BlockBuilder data_block_;
    BlockHandle data_block_handle_;
    // filter block
    FilterBlockBuilder *filter_block_;
    BlockHandle filter_block_handle_;
    // meta index block
    BlockBuilder meta_block_;
    BlockHandle meta_block_handle_;
    // index block
    BlockBuilder index_block_;
    BlockHandle index_block_handle_;
    // footer
    FooterBlockBuilder footer_block_;

    // 缓冲区
    std::string buffer_;
    int offset_;
};
}  // namespace Table
#endif