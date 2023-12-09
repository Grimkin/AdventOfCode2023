find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)

# Add source to this project's executable.
add_executable (AdventOfCode2023 "main.cpp" "Challenge/Day1.cpp" "Challenges.h" "Challenge/Day2.cpp" "Challenge/Utilities.h" "Challenge/Day3.cpp" "Challenge/Day4.cpp" "Challenge/Day5.cpp" "Challenge/Day6.cpp" "Challenge/Day7.cpp" "Challenge/Day8.cpp")
target_link_libraries(AdventOfCode2023 range-v3::range-v3 fmt::fmt)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AdventOfCode2023 PROPERTY CXX_STANDARD 23)
endif()
