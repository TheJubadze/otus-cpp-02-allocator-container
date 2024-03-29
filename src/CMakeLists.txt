cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(container-alloc-cli VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(${PROJECT_NAME}
        main.cpp
        Log.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/spdlog/include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

IF (WIN32)
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/dev/boost_1_77_0")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/dev/boost_1_77_0/lib")
ENDIF ()

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS("-DHAS_BOOST")
ENDIF ()


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Vitaly Senokosov")

include(CPack)