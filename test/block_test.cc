
#include <gtest/gtest.h>
#include "../table/filter_block.h"
#include "../util/bloom.h"
/*

TEST(mmtable_test, BlockBuilder) {
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

    bb.lookup();
}
*/

TEST(block_test, Filter_Block) {
    // 100 ，500 ，600 的偏移量都在第一个 filterblock 中(<2K)
    BloomFilter bloom(10);
    FilterBlockBuilder builder(&bloom);

    builder.BuildBlocks(100);
    builder.AddKey("key1");
    builder.AddKey("key2");
    builder.BuildBlocks(500);
    builder.AddKey("key3");
    std::string filterblock = builder.Finish();

    FilterBlockReader reader(&bloom, filterblock);
    ASSERT_TRUE(reader.KeyMayMatch(100, "key1"));
    ASSERT_TRUE(reader.KeyMayMatch(600, "key2"));
    ASSERT_TRUE(reader.KeyMayMatch(100, "key3"));
}

TEST(block_test, Filter_Blocks) {
    BloomFilter bloom(10);
    FilterBlockBuilder builder(&bloom);

    // 第1个filter block，小于2048
    builder.BuildBlocks(0);
    builder.AddKey("key1");
    builder.BuildBlocks(2000);

    // 第2个filter block，小于4096
    builder.BuildBlocks(4000);
    builder.AddKey("key2");

    // 第3个filter block，空，小于6144

    // 第4个filter block，小于8192
    builder.BuildBlocks(8000);
    builder.AddKey("key3");

    std::string filterblock = builder.Finish();

    FilterBlockReader reader(&bloom, filterblock);

    // 第1个filter block
    ASSERT_TRUE(reader.KeyMayMatch(0, "key1"));
    ASSERT_TRUE(!reader.KeyMayMatch(1000, "key2"));
    ASSERT_TRUE(!reader.KeyMayMatch(2000, "key3"));

    // 2
    ASSERT_TRUE(!reader.KeyMayMatch(4000, "key1"));
    ASSERT_TRUE(reader.KeyMayMatch(4000, "key2"));
    ASSERT_TRUE(!reader.KeyMayMatch(4000, "key3"));

    // 3
    ASSERT_TRUE(!reader.KeyMayMatch(5000, "key1"));
    ASSERT_TRUE(!reader.KeyMayMatch(6000, "key2"));
    ASSERT_TRUE(!reader.KeyMayMatch(6000, "key3"));

    // 4
    ASSERT_TRUE(!reader.KeyMayMatch(8000, "key1"));
    ASSERT_TRUE(!reader.KeyMayMatch(8000, "key2"));
    ASSERT_TRUE(reader.KeyMayMatch(8000, "key3"));
}
