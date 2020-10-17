cmake_minimum_required(VERSION 3.17)
project(milestone1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(milestone1 src/Main.cpp src/Agent.cpp src/Agent.h src/World.cpp src/World.h src/Rand.h)
