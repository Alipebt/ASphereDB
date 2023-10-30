cmake_minimum_required(VERSION 3.10)
project(ASphereDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++2a -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++2a -g -O2")

include_directories(
    ${PROJECT_SOURCE_DIR}/table
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

add_executable(memtable_test table/memtable.cc test/memtable_test.cc)
target_link_libraries(memtable_test gtest gtest_main)

configure_file(test.sh ${CMAKE_CURRENT_BINARY_DIR}/test.sh COPYONLY)
file(COPY test.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

add_test(NAME memtable_test COMMAND memtable_test)


