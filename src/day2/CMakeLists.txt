add_executable(day2_part1)
target_sources(day2_part1 PRIVATE part1.cpp)
target_link_libraries(
  day2_part1
  PUBLIC project_options project_warnings
)

add_executable(day2_part2)
target_sources(day2_part2 PRIVATE part2.cpp)
target_link_libraries(
  day2_part2
  PUBLIC project_options project_warnings
)

