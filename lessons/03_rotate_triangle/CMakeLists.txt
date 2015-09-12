cmake_minimum_required(VERSION 2.8)

PROJECT(03_rotate_triangle)

set(SOURCE main.cpp)

add_executable(03_rotate_triangle ${SOURCE})

target_link_libraries(03_rotate_triangle ${COMMON_LIBS})
