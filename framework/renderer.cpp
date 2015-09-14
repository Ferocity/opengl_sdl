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

#include "framework/renderer.h"
#include <assert.h>

Renderer::Renderer()
    : m_window(nullptr),
      m_glrenderContext(nullptr)
{

}

Renderer::~Renderer()
{

}

bool Renderer::init(const std::string& windowName)
{
    // Initialize SDL
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS))
    {
        fprintf(stderr,"Could not initialize video system: %s\n", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(
                windowName.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                );

    if(!m_window)
    {
        fprintf(stderr,"Could not create window: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool Renderer::deInit()
{
    assert(m_window);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return true;
}

bool Renderer::createContext()
{
    assert(m_window);
    m_glrenderContext = SDL_GL_CreateContext(m_window);

#ifdef OPENGL
    glewExperimental = true;	// fix crash on mac os
    if(GLEW_OK != glewInit())
    {
        fprintf(stderr,"GLEW could not be initialized!\n");
        return false;
    }
#endif

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    return true;
}

bool Renderer::destroyContext()
{
    assert(m_glrenderContext);
    SDL_GL_DeleteContext(m_glrenderContext);
    return true;
}

void Renderer::swapBuffer()
{
    assert(m_window);
    SDL_GL_SwapWindow(m_window);
}

bool Renderer::createProgram(const std::string& name,
                   Shader *vertex,
                   Shader *fragment)
{

    auto iter = m_programs.find(name);
    if(iter != m_programs.end())
    {
        // Already loaded
        return false;
    }

    struct GLSLProgram prog = {
        .s_vertex  = vertex,
        .s_fragment = fragment,
        .s_programId = 0
    };

    m_programs[name] = prog;

    return true;
}


bool Renderer::loadProgram(const std::string& name)
{
    // FIXME better error handling, use exceptions instead of goto's
    auto iter = m_programs.find(name);
    if(iter == m_programs.end())
    {
        // Not found
        return false;
    }

    struct GLSLProgram &program = iter->second;
    program.s_vertex->load();
    program.s_fragment->load();

    int vertexShader = iter->second.s_vertex->getId();
    int fragmentShader = iter->second.s_fragment->getId();

    if( vertexShader == 0 || fragmentShader == 0)
    {
        goto cleanup;
    }

    program.s_programId = glCreateProgram();

    if(0 == program.s_programId)
    {
        goto cleanup;
    }

    glAttachShader(program.s_programId, vertexShader);
    glAttachShader(program.s_programId, fragmentShader);

    // Bind a_position to attribute 0
    glBindAttribLocation(program.s_programId, 0, "a_position");
    // Bind a_color to attribute 1
    glBindAttribLocation(program.s_programId, 1, "a_color");

    glLinkProgram(program.s_programId);

    GLint linked;
    glGetProgramiv(program.s_programId, GL_LINK_STATUS, &linked);
    if(0 == linked)
    {
        GLint errLen = 0;
        glGetProgramiv(program.s_programId, GL_INFO_LOG_LENGTH, &errLen);
        if(1 < errLen)
        {
            GLchar errorLog[errLen];
            glGetProgramInfoLog(program.s_programId, errLen, NULL, errorLog);
            fprintf(stderr,"Error: %s\n", errorLog);
        }
        glDeleteProgram(program.s_programId);
        return false;
    }

cleanup:

    if(vertexShader != 0)
    {
        glDeleteShader(vertexShader);
    }
    if(fragmentShader != 0)
    {
        glDeleteShader(fragmentShader);
    }

    return true;	// FIXME could also be error, atm!
}

unsigned int Renderer::getProgram(const std::string& name)
{
    auto iter = m_programs.find(name);
    if(iter == m_programs.end())
    {
        // Not found
        return 0;
    }
    return iter->second.s_programId;
}


void Renderer::unloadProgram(const std::string& name)
{
    auto iter = m_programs.find(name);
    if(iter == m_programs.end())
    {
        // Not found
        return;
    }
    glDeleteProgram(iter->second.s_programId);
    iter->second.s_programId = 0;
}
