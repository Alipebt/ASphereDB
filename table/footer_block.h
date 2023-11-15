#ifndef FOOTER_BLOCK_H_
#define FOOTER_BLOCK_H_
#include <stdint.h>
#include <string>
#include "../include/status.h"
#include "block.h"

class FooterBlockBuilder {
   public:
    void Build(const BlockHandle &mateindex_block,
               const BlockHandle &index_block);
    std::string Finish();

    void Encode(std::string *dst);
    STATUS Decode(const std::string &src);

   private:
    BlockHandle metaindex_block_;
    BlockHandle index_block_;
};
#endif