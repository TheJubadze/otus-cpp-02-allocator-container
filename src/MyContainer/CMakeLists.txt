cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

add_library(container SHARED
        MyContainer.h
        ../Core.h)

set_target_properties(container PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(container PRIVATE ../"${CMAKE_CURRENT_LIST_DIR}")

if (WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
    add_compile_definitions(BUILD_DLL)
else ()
endif ()