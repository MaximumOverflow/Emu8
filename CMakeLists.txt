cmake_minimum_required(VERSION 3.12)
project(Emu8)

set(CMAKE_CXX_STANDARD 17)

add_executable(Emu8 main.cpp Chip8.cpp Chip8.h Debugger.cpp Debugger.h)