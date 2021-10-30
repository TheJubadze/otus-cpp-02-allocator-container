cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

project(main)

add_subdirectory(MyContainer)

add_executable(playground main.cpp)
target_link_libraries(playground container)
target_include_directories(playground PRIVATE
        "${CMAKE_BINARY_DIR}"
        MyContainer)
