cmake_minimum_required(VERSION 2.8.7)
project(Unigen CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(third_party/gflags)

include_directories("${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE unigen_SRC
   "src/*.cpp"
)

add_executable(Unigen ${unigen_SRC})
target_link_libraries(Unigen gflags)
