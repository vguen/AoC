add_library(day9_util OBJECT)
target_sources(day9_util
  PRIVATE
    util.cpp
    util.h
)
target_link_libraries(day9_util
  PRIVATE
    fmt::fmt
)

add_executable(day9_part1)
target_sources(day9_part1 PRIVATE part1.cpp)
target_link_libraries(day9_part1
  PRIVATE
    day9_util
    myproject::project_options
    myproject::project_warnings
)

add_executable(day9_part2)
target_sources(day9_part2 PRIVATE part2.cpp)
target_link_libraries(day9_part2
  PRIVATE
    day9_util
    myproject::project_options
    myproject::project_warnings
)

add_custom_target(day9
  DEPENDS day9_part1 day9_part2
)

