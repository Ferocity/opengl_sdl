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
    Renderer renderApi;

    // Init SDL
    if(false == renderApi.init("03_rotate_triangle"))
    {
        return EXIT_FAILURE;
    }

    // Create Rendercontext
    if(false == renderApi.createContext())
    {
        return EXIT_FAILURE;
    }

    /***********************
     * Start Rendering
     **********************/

    Shader vertexShader(Shader::Type::VERTEX, vertexShaderSrc, sizeof(vertexShaderSrc));
    Shader fragmentShader(Shader::Type::FRAGMENT,fragmentShaderSrc, sizeof(fragmentShaderSrc));

    renderApi.createProgram("DefaultShader", &vertexShader, &fragmentShader);

    if(false == renderApi.loadProgram("DefaultShader"))
    {
        return EXIT_FAILURE;
    }

    unsigned int program = renderApi.getProgram("DefaultShader");

    SDL_Event e;
    while(e.type != SDL_KEYDOWN
          && e.type != SDL_QUIT)
    {
        SDL_PollEvent(&e);

        glClear(GL_COLOR_BUFFER_BIT);

        draw(program);

        renderApi.swapBuffer();
        SDL_Delay(10);
    }

    renderApi.unloadProgram("DefaultShader");

    if(false == renderApi.destroyContext())
    {
        return EXIT_FAILURE;
    }

    if(false == renderApi.deInit())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
