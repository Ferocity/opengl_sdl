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

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <cstdio>

/**
 * In this first lession will be demonstrated:
 * 	- how a window could be opened with
 *  - how a opengl context will be created
 *  - how a simple triangle could be rendered with OpenGL
 *
 * ATTENTION: This example is not OpenGL ES conform!
 */
int main()
{
    // Initialize SDL, load video and event module
    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        fprintf(stderr,"Could not initialize video system: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create a window with opengl support
    SDL_Window* window = SDL_CreateWindow(
        "Example 01_sdl_init",
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

    // Create OpenGl render context
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    // Load MVP matrix
    glMatrixMode(GL_PROJECTION|GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-320,320,240,-240,0,1);

    // setup clear color (black)
    glClearColor(0.0f,0.0f,0.0f,1.0f);


    SDL_Event e;
    float x = 0.0;
    float y = 30.0;
    // main renderloop
    while(e.type != SDL_KEYDOWN
          && e.type != SDL_QUIT)
    {
      // Poll window events
      SDL_PollEvent(&e);

      // clean backbuffer
      glClear(GL_COLOR_BUFFER_BIT);

      // use old functions to generate a simple triangle
      // old fixed shader pipelines will be used, they are not supported in OpenGl ES
      glRotatef(5.0,0.0,0.0,1.0);
      glBegin(GL_TRIANGLES);
        glVertex2f(x, 	     y + 90.0); glColor3f(1.0, 0.0, 0.0);
        glVertex2f(x + 90.0, y - 90.0); glColor3f(0.0, 1.0, 0.0);
        glVertex2f(x - 90.0, y - 90.0); glColor3f(0.0, 0.0, 1.0);
      glEnd();

      // Swap Front/Backbuffer
      SDL_GL_SwapWindow(window);
      SDL_Delay(10);
    }

    // Cleanup render context
    SDL_GL_DeleteContext(glcontext);
    // Destroy window
    SDL_DestroyWindow(window);
    // Shutdown subsystems
    SDL_Quit();
    return EXIT_SUCCESS;
}
