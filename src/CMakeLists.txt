cmake_minimum_required(VERSION 3.8)
project(sudokist)

add_definitions(-Wall -std=c99)
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    add_definitions(-g)
endif()
add_executable(sudokist sudoku.c main.c)
