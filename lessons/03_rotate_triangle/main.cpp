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

#include <cstdio>


const GLchar vertexShaderSrc[] =
"attribute vec4 a_position; \n"
"attribute vec4 a_color;	\n"
"                           \n"
"varying vec4 v_color;		\n"
"                           \n"
"void main()				\n"
"{							\n"
"	v_color = a_color; 		\n"
"   gl_Position = a_position; \n"
"}							\n";

const GLchar fragmentShaderSrc[] =
"varying vec4 v_color;		\n"
" 							\n"
"void main()				\n"
"{ 							\n"
"	gl_FragColor = v_color; \n"
"} 							\n";


/*
 * load and compile source shader
*/
int loadShader(GLenum type, const GLchar* shaderSrc)
{
    GLuint shader = glCreateShader(type);
    if(0 == shader)
    {
        fprintf(stderr,"Shader creation failed!\n");
        return 0;
    }

    glShaderSource(shader, 1, &shaderSrc, NULL);

    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(0 == compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            GLchar errorLog[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, errorLog);
            fprintf(stderr,"Error: %s\n", errorLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int loadGLSLProgram()
{
    GLuint program = 0;

    int vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSrc);
    int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

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

void draw(int programID)
{
    static const GLfloat vVertices[] = {
         -0.5f, -0.5f, 0.0f,	/*x,y,z*/
         0.5f, -0.5f, 0.0f,
         0.0f, 0.5f, 0.0f
    };
    static const GLfloat vColor[] = {
         1.0f, 0.0f, 0.0f,	/*r,g,b*/
         0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 1.0f
    };

    glUseProgram(programID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColor);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int main()
{
    // Initialize SDL
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS))
    {
        fprintf(stderr,"Could not initialize video system: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow(
                "Example 02_load_shader",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                );

    if(!window)
    {
        fprintf(stderr,"Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

#ifdef OPENGL
    glewExperimental = true;	// fix crash on mac os
    if(GLEW_OK != glewInit())
    {
        fprintf(stderr,"GLEW could not be initialized!\n");
        return EXIT_FAILURE;
    }
#endif

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    SDL_Event e;
    float x = 0.0, y = 30.0;

    int program = loadGLSLProgram();
    if(0 == program)
    {
        return EXIT_FAILURE;
    }

    while(e.type != SDL_KEYDOWN
          && e.type != SDL_QUIT)
    {

        SDL_PollEvent(&e);

        glClear(GL_COLOR_BUFFER_BIT);

        draw(program);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    glDeleteProgram(program);

    SDL_GL_DeleteContext(glcontext);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
