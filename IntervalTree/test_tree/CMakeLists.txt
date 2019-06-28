cmake_minimum_required (VERSION 3.6)

project (test)

include_directories(../include)
add_executable(test_tree interval_tree_test.cpp)
target_link_libraries(test_tree IntervalTree)
