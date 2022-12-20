add_executable(day3_part1)
target_sources(day3_part1 PRIVATE part1.cpp)
target_link_libraries(
  day3_part1
  PUBLIC project_options project_warnings
)

add_executable(day3_part2)
target_sources(day3_part2 PRIVATE part2.cpp)
target_link_libraries(
  day3_part2
  PUBLIC project_options project_warnings
)

