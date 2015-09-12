cmake_minimum_required(VERSION 2.8)

PROJECT(01_sdl_init)

set(SOURCE main.cpp)

add_executable(01_sdl_init ${SOURCE})

target_link_libraries(01_sdl_init SDL2)
