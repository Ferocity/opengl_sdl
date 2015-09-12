cmake_minimum_required(VERSION 2.8)

if(USE_OPENGL)
set(GL_LIBS
         GLEW
         GL
        )
endif()


set(COMMON_LIBS
        SDL2
        ${GL_LIBS}
        glsdl_fw
)

add_subdirectory(01_sdl_init)
add_subdirectory(02_load_shader)
add_subdirectory(03_rotate_triangle)
