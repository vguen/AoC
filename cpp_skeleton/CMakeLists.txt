project( CXX)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(project_options INTERFACE)
# For clang
# target_compile_options(project_options INTERFACE -Weverything)
# For gcc
target_compile_options(project_options INTERFACE
  -Wall -Wextra -Weffc++
)

target_compile_options(project_options INTERFACE -std=c++${CMAKE_CXX_STANDARD_REQUIRED})

target_compile_options(project_options INTERFACE -fsanitize=address,undefined)
target_link_options(project_options INTERFACE -fsanitize=address,undefined)

add_library(project::options ALIAS project_options)

add_executable(part1)
target_sources(part1 PRIVATE part1.cpp)
target_link_libraries(part1 PRIVATE project::options)

add_executable(part2)
target_sources(part2 PRIVATE part2.cpp)
target_link_libraries(part2 PRIVATE project::options)

