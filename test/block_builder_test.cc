#include "../table/block_builder.h"
#include <gtest/gtest.h>

TEST(block_builder_test, Add) {
    BlockBuilder bb;
    bb.Add("key_aaaa", "value001");
    bb.Add("key_aabb", "value002");
    bb.Add("key_bbbb", "value003");
    bb.Add("key_bbbb", "value004");
    bb.Add("key_bbbb", "value005");
    bb.Add("key_bbbb", "value006");
    bb.Add("key_bbbb", "value007");
    bb.Add("key_bbbb", "value008");
    bb.Add("key_bbbb", "value009");
    bb.Add("key_bbbb", "value010");
    bb.Add("key_bbbb", "value011");
    bb.Add("key_bbbb", "value012");
    bb.Add("key_bbbb", "value013");
    bb.Add("key_bbbb", "value014");
    bb.Add("key_bbbb", "value015");
    bb.Add("key_bbbb", "value016");
    // 16 结束
    bb.Add("key_cccc", "value017");
    bb.Add("key_dddd", "value018");

    // bb.lookup();
}