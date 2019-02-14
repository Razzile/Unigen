cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(libunigen)

add_executable(unigen unigen.cc)

target_include_directories(unigen PUBLIC ${CMAKE_SOURCE_DIR}/third_party/gflags/src)

target_link_libraries(unigen libunigen-static)