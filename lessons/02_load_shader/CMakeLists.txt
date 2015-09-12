cmake_minimum_required(VERSION 2.8)

PROJECT(02_load_shader)

set(SOURCE main.cpp)

add_executable(02_load_shader ${SOURCE})

target_link_libraries(02_load_shader SDL2 GLEW)
