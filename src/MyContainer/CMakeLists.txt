cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

add_library(container MyContainer.h)

set_target_properties(container PROPERTIES LINKER_LANGUAGE CXX)
