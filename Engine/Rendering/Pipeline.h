#ifndef EN_PIPELINE_H
#define EN_PIPELINE_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "DeviceBuffer.h"
#include "Texture.h"

#define PIPELINE_STAGE_COUNT 5


enum class EShaderType : uint16_t {
    Vertex = GL_VERTEX_SHADER,
    TessellationControl = GL_TESS_CONTROL_SHADER,
    TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    None = 0
};


class CPipeline {
private:
    uint32_t MId;
    std::array<uint32_t, PIPELINE_STAGE_COUNT> Stages;

private:
    const char *GetShaderTypeString(EShaderType Type);
    uint8_t GetShaderTypeIndex(EShaderType Type);
    uint32_t Compile(const char *Source, EShaderType Type);
public:
    CPipeline();

	void AddStage(const char *Filepath, EShaderType Type);
    void RemoveStage(EShaderType Type);

    void Bind();
    void Unbind();

    int32_t GetUniformLocation(const char *UniformName);
    void SetUniform(CTexture *Value, const char *UniformName);
    void SetUniform(float Value, const char *UniformName);
    void SetUniform(int32_t Value, const char *UniformName);
    void SetUniform(uint32_t Value, const char *UniformName);
    void SetUniform(glm::vec2 &Value, const char *UniformName);
    void SetUniform(glm::vec3 &Value, const char *UniformName);
    void SetUniform(glm::vec4 &Value, const char *UniformName);
    void SetUniform(glm::ivec2 &Value, const char *UniformName);
    void SetUniform(glm::ivec3 &Value, const char *UniformName);
    void SetUniform(glm::ivec4 &Value, const char *UniformName);
    void SetUniform(glm::uvec2 &Value, const char *UniformName);
    void SetUniform(glm::uvec3 &Value, const char *UniformName);
    void SetUniform(glm::uvec4 &Value, const char *UniformName);
    void SetUniform(glm::mat2 &Value, const char *UniformName);
    void SetUniform(glm::mat2x3 &Value, const char *UniformName);
    void SetUniform(glm::mat2x4 &Value, const char *UniformName);
    void SetUniform(glm::mat3x2 &Value, const char *UniformName);
    void SetUniform(glm::mat3 &Value, const char *UniformName);
    void SetUniform(glm::mat3x4 &Value, const char *UniformName);
    void SetUniform(glm::mat4x2 &Value, const char *UniformName);
    void SetUniform(glm::mat4x3 &Value, const char *UniformName);
    void SetUniform(glm::mat4 &Value, const char *UniformName);

    ~CPipeline();
};

#endif

