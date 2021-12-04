cmake_minimum_required(VERSION 3.10...3.21 FATAL_ERROR)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(container-alloc-cli VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_subdirectory(src)
