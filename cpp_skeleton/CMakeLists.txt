add_library(dayX_util OBJECT)
target_sources(dayX_util
  PRIVATE
    util.cpp
    util.h
)

add_executable(dayX_part1)
target_sources(dayX_part1 PRIVATE part1.cpp)
target_link_libraries(dayX_part1
  PRIVATE
    dayX_util
    myproject::project_options
    myproject::project_warnings
)

add_executable(dayX_part2)
target_sources(dayX_part2 PRIVATE part2.cpp)
target_link_libraries(dayX_part2
  PRIVATE
    dayX_util
    myproject::project_options
    myproject::project_warnings
)

add_custom_target(dayX
  DEPENDS dayX_part1 dayX_part2
)

