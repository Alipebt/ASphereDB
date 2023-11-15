#include "../db/db.h"
#include <gtest/gtest.h>

TEST(db_test, PG) {
    DB *db;
    std::string value;
    ASSERT_EQ(DB::Create("dbtest"), S_OK);
    ASSERT_EQ(DB::Open("dbtest", &db), S_OK);
    ASSERT_EQ(db->Put("key", "value"), S_OK);
    ASSERT_EQ(db->Get("key", value), S_OK);
    ASSERT_EQ(value, "value");
    ASSERT_EQ(db->Delete("key"), S_OK);
    ASSERT_EQ(db->Get("key", value), S_OK);
    ASSERT_EQ(DB::Destory("dbtest"), S_OK);
    delete db;
}
