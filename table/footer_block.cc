#include "footer_block.h"
#include "../util/coding.h"

void FooterBlockBuilder::Build(const BlockHandle &metaindex_block,
                               const BlockHandle &index_block) {
    metaindex_block_ = metaindex_block;
    index_block_ = index_block;
}
std::string FooterBlockBuilder::Finish() {
    std::string result;
    // result.append(metaindex_block_.Decode());
    return result;
}
void FooterBlockBuilder::Encode(std::string *dst) {
    metaindex_block_.Encode(dst);
    index_block_.Encode(dst);
}
STATUS FooterBlockBuilder::Decode(const std::string &src) {
    STATUS ret = metaindex_block_.Decode(src);
    if (ret == S_OK) {
        ret = index_block_.Decode(src);
    }
    return ret;
}