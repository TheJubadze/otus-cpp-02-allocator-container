cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

add_library(allocator MyAllocator.h)

set_target_properties(allocator PROPERTIES LINKER_LANGUAGE CXX)
