cmake_minimum_required(VERSION 2.8)


set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${MODULE_PATH})

if("${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
    set(CMAKE_TOOLCHAIN_FILE "${MODULE_PATH}/linux-gcc-x64.toolchain")
endif()
MESSAGE(STATUS "Using Toolchainfile: ${CMAKE_TOOLCHAIN_FILE}")


SET(PROJECT_NAME "opengl_sdl")

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

project(${PROJECT_NAME})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(USE_OPENGL "Activate opengl usage" ON)

if(USE_OPENGL)
add_definitions(-DOPENGL)
endif()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/SDL/include/
        ${CMAKE_BINARY_DIR}/GLEW/usr/include/
        )

link_directories(
        ${CMAKE_BINARY_DIR}/SDL/lib/
        ${CMAKE_BINARY_DIR}/GLEW/usr/lib64
        ${CMAKE_BINARY_DIR}/GLEW/usr/lib
        )

add_subdirectory(thirdparty)
add_subdirectory(lessons)
add_subdirectory(framework)

