add_library(day8_util OBJECT)
target_sources(day8_util
  PRIVATE
    util.cpp
    util.h
)
add_executable(day8_part1)
target_sources(day8_part1 PRIVATE part1.cpp)
target_link_libraries(day8_part1
  PRIVATE
    myproject::project_options
    myproject::project_warnings
    day8_util
)

add_executable(day8_part2)
target_sources(day8_part2 PRIVATE part2.cpp)
target_link_libraries(day8_part2
  PRIVATE
    myproject::project_options
    myproject::project_warnings
    day8_util
)

add_custom_target(day8
  DEPENDS day8_part1 day8_part2
)

