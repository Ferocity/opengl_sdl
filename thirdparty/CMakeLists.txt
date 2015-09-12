#The MIT License (MIT)
#
#Copyright (c) 2014 Christian Fries
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

# SDL TEST Suite

set(SDL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/SDL")
set(SDL_INSTALL_DIR "${CMAKE_BINARY_DIR}/SDL")
set(SDL_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SDL_INSTALL_DIR})

ExternalProject_Add(SDL
  PREFIX ${SDL_PREFIX}
  URL https://www.libsdl.org/release/SDL2-2.0.3.tar.gz
  BUILD_COMMAND make all COMMAND echo done
#  UPDATE_COMMAND ""
  INSTALL_COMMAND make install
  CMAKE_ARGS ${SDL_CMAKE_ARGS}
)
