# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "conan_provider.cmake")

project ("AdventOfCode2023")

set(CMAKE_CXX_STANDARD 20)

# Include sub-projects.
add_subdirectory ("AdventOfCode2023")
