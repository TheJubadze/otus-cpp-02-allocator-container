cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

project(main)

add_executable(playground main.cpp)

target_include_directories(playground PUBLIC "${PROJECT_SOURCE_DIR}")

