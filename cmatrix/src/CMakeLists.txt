cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 17)

project(cmatrix C)

add_subdirectory("matrix")

add_executable(app "main.c")

target_link_libraries(app matrix m)
