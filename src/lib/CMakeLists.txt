add_library(MyAllocator STATIC MyAllocator.cpp)

target_include_directories(MyAllocator PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../spdlog/include)
