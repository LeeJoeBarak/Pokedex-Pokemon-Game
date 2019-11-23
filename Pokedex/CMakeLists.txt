cmake_minimum_required(VERSION 3.13)
project(Pokedex C)

set(CMAKE_C_STANDARD 99)

add_executable(Pokedex main.c Pokemon.h Pokemon.c cmake-build-debug/Defs.h)