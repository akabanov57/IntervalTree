cmake_minimum_required (VERSION 3.6)

project (IntervalTree)

include_directories(../include)
add_library(IntervalTree SHARED IntervalTree.cpp)
