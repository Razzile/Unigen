cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(LIEF_PYTHON_API OFF CACHE BOOL "don't use python" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/gflags third_party/gflags)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/capstone third_party/capstone)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/LIEF third_party/LIEF)


include_directories(".")

file(GLOB_RECURSE unigen_SRC "*.cc" "*.h")

add_library(libunigen OBJECT ${unigen_SRC})

set_property(TARGET ${libunigen} PROPERTY POSITION_INDEPENDENT_CODE 1)

# set_property(TARGET Unigen PROPERTY CXX_STANDARD 17)

target_include_directories(libunigen PUBLIC
        ${CMAKE_SOURCE_DIR}/third_party/capstone/include
        ${CMAKE_SOURCE_DIR}/third_party/gflags/src
        ${CMAKE_SOURCE_DIR}/third_party/LIEF/include
        ${CMAKE_BINARY_DIR}/third_party/LIEF/include)

add_library(libunigen-shared SHARED $<TARGET_OBJECTS:libunigen>)
add_library(libunigen-static STATIC $<TARGET_OBJECTS:libunigen>)

target_link_libraries(libunigen-shared gflags capstone-static LIB_LIEF_STATIC)
target_link_libraries(libunigen-static gflags capstone-static LIB_LIEF_STATIC)