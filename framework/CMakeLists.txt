cmake_minimum_required(VERSION 2.8)

PROJECT(Framework)

set(SOURCE
        shader
        renderer
)

add_library(glsdl_fw ${SOURCE})

