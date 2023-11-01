cmake_minimum_required(VERSION 3.10)
project(ASphereDBImpl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++2a -g -fno-omit-frame-pointer  -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++2a -g -O2")

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
add_executable(memtable_test table/memtable.cc test/memtable_test.cc)
target_link_libraries(memtable_test gtest gtest_main)

add_executable(file_manager_test db/file_manager.cc test/file_manager_test)
target_link_libraries(file_manager_test gtest gtest_main)

add_executable(db_test db/db.cc db/file_manager.cc table/memtable.cc test/db_test)
target_link_libraries(db_test gtest gtest_main )

# 添加测试
add_test(NAME memtable_test COMMAND memtable_test)
add_test(NAME file_manager_test COMMAND file_manager_test)
add_test(NAME db_test COMMAND db_test)


