cmake_minimum_required(VERSION 3.23)
project(battleship)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)


add_executable(battleship main.c console.c console.h)