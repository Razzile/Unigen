cmake_minimum_required(VERSION 3.1)
project(Unigen CXX)

set(CMAKE_CXX_STANDARD 11)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

option(BUILD_TESTS "Build unigen tests" ON)

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
