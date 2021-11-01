cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

project(main)

add_executable(playground main.cpp)

#add_subdirectory(MyContainer)
#add_subdirectory(MyAllocator)
#target_link_libraries(playground container)

target_include_directories(playground PUBLIC
        "${PROJECT_SOURCE_DIR}"
        MyContainer
        MyAllocator)

#if (WIN32)
#    add_compile_definitions(PLATFORM_WINDOWS)
#    add_compile_definitions(BUILD_DLL)
#else ()
#endif ()
