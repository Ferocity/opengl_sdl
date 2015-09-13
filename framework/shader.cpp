#include "framework/shader.h"
#include "framework/renderer.h"

#include <cstdio>
#include <cstring>


Shader::Shader(Type type, const char source[], std::size_t size)
    : m_data(size),
      m_type(type),
      m_shaderId(0)
{
    ::memcpy(&m_data[0],source,size);
}

Shader::~Shader()
{

}

/*
 * load and compile source shader
*/
bool Shader::load()
{
    if(Type::VERTEX == m_type)
    {
        m_shaderId = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(Type::FRAGMENT == m_type)
    {
        m_shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        fprintf(stderr,"Unknown shader type!\n");
        return false;
    }


    if(0 == m_shaderId)
    {
        fprintf(stderr,"Shader creation failed!\n");
        return false;
    }

    glShaderSource(m_shaderId, 1, reinterpret_cast<const char**>(&m_data), NULL);

    glCompileShader(m_shaderId);

    GLint compiled;
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &compiled);
    if(0 == compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            GLchar errorLog[infoLen];
            glGetShaderInfoLog(m_shaderId, infoLen, NULL, errorLog);
            fprintf(stderr,"Error: %s\n", errorLog);
        }
        glDeleteShader(m_shaderId);
        return false;
    }
    return true;
}

bool Shader::unload()
{
    glDeleteShader(m_shaderId);
    m_shaderId = 0;
    return true;
}
