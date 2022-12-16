project(day2 CXX)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_options INTERFACE)
# For clang
# target_compile_options(project_options INTERFACE -Weverything)
# For gcc
target_compile_options(project_options INTERFACE -Wall -Wextra -Weffc++ -std=c++${CMAKE_CXX_STANDARD_REQUIRED})
target_compile_options(project_options INTERFACE -fsanitize=address)
target_link_options(project_options INTERFACE -fsanitize=address)
add_library(project::options ALIAS project_options)

add_executable(part1)
target_sources(part1 PRIVATE part1.cpp)
target_link_libraries(part1 PRIVATE project::options)

add_executable(part2)
target_sources(part2 PRIVATE part2.cpp)
target_link_libraries(part2 PRIVATE project::options)
