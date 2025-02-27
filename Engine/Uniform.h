#ifndef PE_UNIFORM_H
#define PE_UNIFORM_H

#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class CUniform {
private:
    static GLint GetUniformLocation(CShader *Shader, std::string Name);
public:
    template<typename T>
    static void SetUniform(CShader *Shader, std::string Name, T Value);
};

#endif