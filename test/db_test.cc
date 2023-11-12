#include "../db/db.h"
#include <gtest/gtest.h>
#include "../db/file_manager.h"

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

TEST(db_test, FileManager) {
    FileManager fm;
    ASSERT_EQ(fm.Create("dirtest", T_DIR), S_OK) << "Create dir error";
    ASSERT_EQ(fm.Exists("dirtest"), S_OK) << "Exitst error";
    ASSERT_EQ(fm.Delete("dirtest"), S_OK) << "Delete dir error";
}