cmake_minimum_required(VERSION 3.5)
project(simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -fopenmp")
set(CMAKE_CXX_COMPILER=g++)

add_executable(simulation src/Main.cpp src/Agent.cpp src/Agent.h src/World.cpp src/World.h src/Rand.h)
