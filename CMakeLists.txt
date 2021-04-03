cmake_minimum_required(VERSION 3.17)
project(Final)

set(CMAKE_CXX_STANDARD 14)

add_executable(WordSolver main.cpp "header.h")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/WordsByLength.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")