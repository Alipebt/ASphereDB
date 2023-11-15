cmake_minimum_required(VERSION 3.10)
project(ASphereDBImpl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++2a -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++2a -g -O2")
set(CMAKE_CXX_FLAGS "-fsanitize=address")

# 头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/table
    ${PROJECT_SOURCE_DIR}/include
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

# 复制test.sh文件
configure_file(test.sh ${CMAKE_CURRENT_BINARY_DIR}/test.sh COPYONLY)
file(COPY test.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# 为测试文件添加链接
# table_test
add_executable(table_test  test/table_test.cc 
                table/memtable.cc
                table/block.cc
                table/filter_block.cc
                util/coding.cc
                util/bloom.cc
                util/hash.cc)
target_link_libraries(table_test gtest gtest_main)

# db_test
add_executable(db_test  test/db_test.cc db/db.cc 
                table/memtable.cc)
target_link_libraries(db_test gtest gtest_main )

# util_test
add_executable(util_test test/util_test.cc
                util/hash.cc 
                util/bloom.cc
                util/coding.cc
                util/file_manager.cc)
target_link_libraries(util_test gtest gtest_main )

# 添加测试
add_test(NAME table_test COMMAND table_test)
add_test(NAME db_test COMMAND db_test)
add_test(NAME util_test COMMAND util_test)


