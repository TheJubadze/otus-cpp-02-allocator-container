cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

project(main)

add_executable(${PROJECT_NAME}
        main.cpp
        MyContainer.h
        MyAllocator.h
        Log.h
        Log.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

IF (WIN32)
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/dev/boost_1_77_0")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/dev/boost_1_77_0/lib")
ENDIF ()

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS("-DHAS_BOOST")
ENDIF ()
