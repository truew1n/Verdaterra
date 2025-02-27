#ifndef EN_SHADER_H
#define EN_SHADER_H

#include <iostream>
#include <glad/glad.h>

#include "File.h"

#include "RenderObject.h"

enum class EShaderType : uint32_t {
    Vertex = GL_VERTEX_SHADER,
    TessellationControl = GL_TESS_CONTROL_SHADER,
    TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Compute = GL_COMPUTE_SHADER
};

class CShader : public CRenderObject {
private:
    const char *MatchShaderTypeString(EShaderType Type);
public:
    uint32_t Compile(const char *Source, EShaderType Type);
    void Create();
    uint8_t Load(const char *Filepath, EShaderType Type);
    void Activate();
    void Deactivate();
    void Delete();
};

#endif