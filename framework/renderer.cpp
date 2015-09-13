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
        .s_fragment = fragment
    };

    m_programs[name] = prog;

    return true;
}


unsigned int Renderer::loadProgram(const std::string& name)
{
    // FIXME better error handling, use exceptions instead of goto's
    GLuint program = 0;

    auto iter = m_programs.find(name);
    if(iter == m_programs.end())
    {
        // Not found
        return 0;
    }

    iter->second.s_vertex->load();
    iter->second.s_fragment->load();

    int vertexShader = iter->second.s_vertex->getId();
    int fragmentShader = iter->second.s_fragment->getId();

    if( vertexShader == 0 || fragmentShader == 0)
    {
        goto cleanup;
    }

    program = glCreateProgram();

    if(0 == program)
    {
        goto cleanup;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Bind a_position to attribute 0
    glBindAttribLocation(program, 0, "a_position");
    // Bind a_color to attribute 1
    glBindAttribLocation(program, 1, "a_color");

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(0 == linked)
    {
        GLint errLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errLen);
        if(1 < errLen)
        {
            GLchar errorLog[errLen];
            glGetProgramInfoLog(program, errLen, NULL, errorLog);
            fprintf(stderr,"Error: %s\n", errorLog);
        }
        glDeleteProgram(program);
        return 0;
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
    return program;
}

void Renderer::unloadProgram(const std::string& name)
{
    // TODO
}
