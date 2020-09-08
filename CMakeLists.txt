cmake_minimum_required(VERSION 3.17)
project(milestone1)

set(CMAKE_CXX_STANDARD 11)
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")

add_executable(milestone1 Main.cpp Main.cpp Agent.cpp Agent.h World.cpp World.h Rand.h)