#ifndef SHADER_H
#define SHADER_H

#include <vector>

class Shader
{
public:
    enum class Type
    {
        VERTEX,
        FRAGMENT
    };

    Shader(Type type,const char source[], std::size_t size);
    ~Shader();

    bool load();
    bool unload();

    inline unsigned int getId() { return m_shaderId; }
private:
    std::vector<char> m_data;
    Type m_type;
    unsigned int m_shaderId;
};


#endif // SHADER_H
