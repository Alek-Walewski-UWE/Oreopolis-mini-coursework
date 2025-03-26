cmake_minimum_required(VERSION 3.21)
project(Oreopolis_mini_coursework C)

set(CMAKE_C_STANDARD 11)

include_directories(Code)

add_executable(Oreopolis_mini_coursework
        Code/main.c
        Code/main.h)
