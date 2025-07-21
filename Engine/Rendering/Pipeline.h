#ifndef EN_PIPELINE_H
#define EN_PIPELINE_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "Handle.h"
#include "DeviceBuffer.h"
#include "Texture.h"

#define EN_PIPELINE_STAGE_COUNT 5


enum class EShaderType : uint16_t {
    Vertex = GL_VERTEX_SHADER,
    TessellationControl = GL_TESS_CONTROL_SHADER,
    TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    None = 0
};

// Not used yet, expansion in future
typedef struct SPipelineState {
    uint8_t DepthTest = 1;
    uint8_t CullFace = 1;
    uint8_t CullBackFace = 1;
    uint8_t CullFaceWindingOrder = 1;
} SPipelineState;


class CPipeline : public CRenderObject, public CHandle {
private:
    std::array<CHandle, EN_PIPELINE_STAGE_COUNT> Stages;

private:
    const char *GetShaderTypeString(EShaderType Type);
    uint8_t GetShaderTypeIndex(EShaderType Type);
    uint32_t Compile(const char *Source, EShaderType Type);
public:
    virtual void Create() override;
    virtual void Bind() override;

    void AddStage(const char *Filepath, EShaderType Type);
    void RemoveStage(EShaderType Type);

    int32_t GetUniformLocation(const char *UniformName);
    void SetUniform(CTexture &Value, const char *UniformName);
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

    virtual void Unbind() override;
    virtual void Destroy() override;
};

#endif

