#include "../table/memtable.h"
#include "../table/result.h"
#include <gtest/gtest.h>

TEST(memtable_test, PG)
{
    MemTable memtable;
    MemKey memkey("key", 0, K_ADD);

    string value;
    ASSERT_EQ(memtable.Put(memkey, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey, value), RE_OK) << "Get error";

    ASSERT_EQ(value, "value1") << "Value error";
}

TEST(memtable_test, PG_same_key)
{
    MemTable memtable;
    MemKey memkey("key", 0, K_ADD);

    string value;
    ASSERT_EQ(memtable.Put(memkey, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey, "value2"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey, value), RE_OK) << "Get error";

    ASSERT_EQ(value, "value2") << "Value error";
}

TEST(memtable_test, PG_diff_key)
{
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Put(memkey1, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, "value2"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey1, value1), RE_OK) << "Get error";
    ASSERT_EQ(memtable.Get(memkey2, value2), RE_OK) << "Get error";

    ASSERT_EQ(value1, "value1") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(memtable_test, Replace_key)
{
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Put(memkey1, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, "value2"), RE_OK) << "Add error";
    memkey1.seq_ = 2;
    ASSERT_EQ(memtable.Put(memkey1, "value3"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey1, value1), RE_OK) << "Get error";
    ASSERT_EQ(memtable.Get(memkey2, value2), RE_OK) << "Get error";

    ASSERT_EQ(value1, "value3") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(memtable_test, Delete_key)
{
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Put(memkey1, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, "value2"), RE_OK) << "Add error";
    memkey1.seq_ = 2;
    memkey1.km_ = K_DELETE;
    memkey2.seq_ = 3;
    memkey2.km_ = K_DELETE;
    ASSERT_EQ(memtable.Put(memkey1, ""), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, ""), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey1, value1), RE_OK) << "Get error";
    ASSERT_EQ(memtable.Get(memkey2, value2), RE_OK) << "Get error";

    ASSERT_EQ(value1, "") << "Value error";
    ASSERT_EQ(value2, "") << "Value error";
}

TEST(memtable_test, Delete_key_reput)
{
    MemTable memtable;
    MemKey memkey1("key1", 0, K_ADD);
    MemKey memkey2("key2", 1, K_ADD);

    string value1, value2;
    ASSERT_EQ(memtable.Put(memkey1, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, "value2"), RE_OK) << "Add error";
    memkey1.seq_ = 2;
    memkey1.km_ = K_DELETE;
    memkey2.seq_ = 3;
    memkey2.km_ = K_DELETE;
    ASSERT_EQ(memtable.Put(memkey1, ""), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, ""), RE_OK) << "Add error";
    memkey1.seq_ = 4;
    memkey1.km_ = K_ADD;
    memkey2.seq_ = 5;
    memkey2.km_ = K_ADD;
    ASSERT_EQ(memtable.Put(memkey1, "value1"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Put(memkey2, "value2"), RE_OK) << "Add error";
    ASSERT_EQ(memtable.Get(memkey1, value1), RE_OK) << "Get error";
    ASSERT_EQ(memtable.Get(memkey2, value2), RE_OK) << "Get error";

    ASSERT_EQ(value1, "value1") << "Value error";
    ASSERT_EQ(value2, "value2") << "Value error";
}

TEST(memtable_test, Big_key)
{
    MemTable memtable;

    for (int i = 0; i < 5000; i++)
    {
        MemKey m("key" + to_string(i), i, K_ADD);
        ASSERT_EQ(memtable.Put(m, "value" + to_string(i)), RE_OK) << "Put error";
    }
    for (int i = 0; i < 5000; i++)
    {
        MemKey m("key" + to_string(i), i, K_ADD);
        string value;
        ASSERT_EQ(memtable.Get(m, value), RE_OK) << "Put error";
        ASSERT_EQ(value, "value" + to_string(i)) << "Value error";
    }
}
