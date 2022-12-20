add_executable(day1)
target_sources(day1 PRIVATE main.cpp)
target_link_libraries(
  day1
  PUBLIC project_options project_warnings
)

