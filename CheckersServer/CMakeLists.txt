cmake_minimum_required(VERSION 3.12)
project(CheckersServer C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)
add_executable(CheckersServer main.c game.c game.h test.c test.h)