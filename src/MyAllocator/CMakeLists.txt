cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

add_library(allocator SHARED MyAllocator.h)

set_target_properties(allocator PROPERTIES LINKER_LANGUAGE CXX)

if (WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
    add_compile_definitions(BUILD_DLL)
else ()
endif ()