#include "../db/file_manager.h"
#include <gtest/gtest.h>

TEST(file_manager_test, Dir) {
    FileManager fm;
    ASSERT_EQ(fm.Create("dirtest", T_DIR), S_OK) << "Create dir error";
    ASSERT_EQ(fm.Exists("dirtest"), S_OK) << "Exitst error";
    ASSERT_EQ(fm.Delete("dirtest"), S_OK) << "Delete dir error";
}