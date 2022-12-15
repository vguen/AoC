project(day1 CXX)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(project_options INTERFACE)
# For clang
# target_compile_options(project_options INTERFACE -Weverything)
# For gcc
target_compile_options(project_options INTERFACE -Wall -Wextra -Weffc++)
add_library(project::options ALIAS project_options)

add_executable(day1)
target_sources(day1 PRIVATE main.cpp)
target_link_libraries(day1 PRIVATE project::options)

