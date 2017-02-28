cmake_minimum_required(VERSION 3.8)
project(sudokist)

add_definitions(-Wall -std=c99)
add_executable(sudokist sudoku.c main.c)
