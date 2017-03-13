cmake_minimum_required(VERSION 3.1)
project(Unigen CXX)

set(CMAKE_CXX_STANDARD 11)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
add_subdirectory(third_party/gflags)

include_directories("${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE unigen_SRC
   "src/*.cc"
   "src/*.h"
)

set_property(TARGET Unigen PROPERTY CXX_STANDARD 17)

add_executable(Unigen ${unigen_SRC})

target_link_libraries(Unigen gflags)
