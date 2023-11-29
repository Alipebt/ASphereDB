#include <gtest/gtest.h>
#include "../table/memtable.h"

using namespace Table;

TEST(mmtable_test, PG) {
    MemTable memtable;
    MemKey memkey("key", 0, K_ADD);

    string value;
    ASSERT_EQ(memtable.Add(memkey, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key", value), S_OK) << "Get error";

    ASSERT_EQ(value, "value1") << "Value error";
}

TEST(mmtable_test, PG_same_key) {
    MemTable memtable;
    MemKey memkey("key", 0, K_ADD);

    string value;
    ASSERT_EQ(memtable.Add(memkey, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey, "value2"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key", value), S_OK) << "Get error";

    ASSERT_EQ(value, "value2") << "Value error";
}

TEST(mmtable_test, PG_diff_key) {
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Add(memkey1, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, "value2"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key1", value1), S_OK) << "Get error";
    ASSERT_EQ(memtable.Get("key2", value2), S_OK) << "Get error";

    ASSERT_EQ(value1, "value1") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(mmtable_test, Replace_key) {
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Add(memkey1, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, "value2"), S_OK) << "Add error";
    memkey1.seq_ = 2;
    ASSERT_EQ(memtable.Add(memkey1, "value3"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key1", value1), S_OK) << "Get error";
    ASSERT_EQ(memtable.Get("key2", value2), S_OK) << "Get error";

    ASSERT_EQ(value1, "value3") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(mmtable_test, Delete_key) {
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Add(memkey1, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, "value2"), S_OK) << "Add error";
    memkey1.seq_ = 2;
    memkey1.kt_ = K_DELETE;
    memkey2.seq_ = 3;
    memkey2.kt_ = K_DELETE;
    ASSERT_EQ(memtable.Add(memkey1, ""), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, ""), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key1", value1), S_OK) << "Get error";
    ASSERT_EQ(memtable.Get("key2", value2), S_OK) << "Get error";

    ASSERT_EQ(value1, "") << "Value error";
    ASSERT_EQ(value2, "") << "Value error";
}

TEST(mmtable_test, Delete_key_reput) {
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Add(memkey1, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, "value2"), S_OK) << "Add error";
    memkey1.seq_ = 2;
    memkey1.kt_ = K_DELETE;
    memkey2.seq_ = 3;
    memkey2.kt_ = K_DELETE;
    ASSERT_EQ(memtable.Add(memkey1, ""), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, ""), S_OK) << "Add error";
    memkey1.seq_ = 4;
    memkey1.kt_ = K_ADD;
    memkey2.seq_ = 5;
    memkey2.kt_ = K_ADD;
    ASSERT_EQ(memtable.Add(memkey1, "value1"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Add(memkey2, "value2"), S_OK) << "Add error";
    ASSERT_EQ(memtable.Get("key1", value1), S_OK) << "Get error";
    ASSERT_EQ(memtable.Get("key2", value2), S_OK) << "Get error";

    ASSERT_EQ(value1, "value1") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(mmtable_test, Big_key) {
    MemTable memtable;

    for (int i = 0; i < 5000; i++) {
        MemKey m("key" + to_string(i), i, K_ADD);
        ASSERT_EQ(memtable.Add(m, "value" + to_string(i)), S_OK) << "Add error";
    }
    for (int i = 0; i < 5000; i++) {
        string value;
        ASSERT_EQ(memtable.Get("key" + to_string(i), value), S_OK)
            << "Add error";
        ASSERT_EQ(value, "value" + to_string(i)) << "Value error";
    }
}
