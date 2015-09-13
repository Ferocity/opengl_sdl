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
    };

public:
    Renderer();
    ~Renderer();

    bool init(const std::string& windowName);
    bool deInit();

    bool createContext();
    bool destroyContext();

    bool createProgram(const std::string& name, Shader *vertex, Shader *fragment);
    unsigned int loadProgram(const std::string& name);
    void unloadProgram(const std::string& name);

    void swapBuffer();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glrenderContext;
    std::unordered_map<std::string, GLSLProgram> m_programs;
};


#endif // RENDERER_H
