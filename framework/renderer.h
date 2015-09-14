/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Christian Fries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef RENDERER_H
#define RENDERER_H

#ifdef OPENGL
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined OPENGLES20
#include <GLES2/gl2.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#else
    #error("Unsupported Platform")
#endif

#include <string>
#include <unordered_map>

#include "framework/shader.h"

class Renderer
{
private:
    struct GLSLProgram
    {
        Shader *s_vertex;
        Shader *s_fragment;
        unsigned int s_programId;
    };

public:
    Renderer();
    ~Renderer();

    bool init(const std::string& windowName);
    bool deInit();

    bool createContext();
    bool destroyContext();

    bool createProgram(const std::string& name, Shader *vertex, Shader *fragment);
    bool loadProgram(const std::string& name);
    unsigned int getProgram(const std::string& name);
    void unloadProgram(const std::string& name);

    void swapBuffer();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glrenderContext;
    std::unordered_map<std::string, GLSLProgram> m_programs;
};


#endif // RENDERER_H
