#include "Pipeline.h"

#include <fstream>
#include <filesystem>

#include "Utils/Logger.h"

const char *CPipeline::GetShaderTypeString(EShaderType Type)
{
    switch (Type) {
        case EShaderType::Vertex:                           return "Vertex Shader";
        case EShaderType::TessellationControl:              return "Tessellation Control Shader";
        case EShaderType::TessellationEvaluation:           return "Tessellation Evaluation Shader";
        case EShaderType::Geometry:                         return "Geometry Shader";
        case EShaderType::Fragment:                         return "Fragment Shader";
        case EShaderType::None:                             return "Unknown Shader Type";
        default:                                            return "Unknown Shader Type";
    }
}

uint8_t CPipeline::GetShaderTypeIndex(EShaderType Type)
{
    switch (Type) {
        case EShaderType::Vertex:                           return 0;
        case EShaderType::TessellationControl:              return 1;
        case EShaderType::TessellationEvaluation:           return 2;
        case EShaderType::Geometry:                         return 3;
        case EShaderType::Fragment:                         return 4;
        case EShaderType::None:                             return EN_PIPELINE_STAGE_COUNT;
        default:                                            return EN_PIPELINE_STAGE_COUNT;
    }
}

uint32_t CPipeline::Compile(const char *Source, EShaderType Type)
{
    uint32_t ShaderId = glCreateShader(static_cast<GLenum>(Type));
    glShaderSource(ShaderId, 1, &Source, nullptr);
    glCompileShader(ShaderId);

    int32_t Result;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

    if (!Result) {
        int32_t Length;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &Length);
        char *Message = new char[Length];
        glGetShaderInfoLog(ShaderId, Length, &Length, Message);
        LOG_ERROR_FORMAT(
            "Failed to compile %s shader!\n%s\n",
            GetShaderTypeString(Type),
            Message
        );
        delete[] Message;
        glDeleteShader(ShaderId);
        return 0;
    }

    return ShaderId;
}

void CPipeline::Create()
{
    MId = glCreateProgram();
    Stages = { };
}

void CPipeline::Bind()
{
    glUseProgram(MId);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void CPipeline::AddStage(const char *Filepath, EShaderType Type)
{
    uint32_t ShaderIndex = GetShaderTypeIndex(Type);
    if (ShaderIndex >= EN_PIPELINE_STAGE_COUNT) {
        return;
    }

    std::ifstream ShaderFile(Filepath, std::ios::binary);
    std::string Content(std::filesystem::file_size(Filepath), '\0');
    ShaderFile.read(&Content[0], Content.size());

    uint32_t ShaderId = Compile(Content.c_str(), Type);

    if (ShaderId == EN_HANDLE_INVALID_ID) {
        return;
    }

    glAttachShader(MId, ShaderId);
    glLinkProgram(MId);
    glValidateProgram(MId);

    Stages[ShaderIndex].SetId(ShaderId);
}

void CPipeline::RemoveStage(EShaderType Type)
{
    uint32_t ShaderIndex = GetShaderTypeIndex(Type);
    if (ShaderIndex >= EN_PIPELINE_STAGE_COUNT) {
        return;
    }

    uint32_t ShaderId = Stages[ShaderIndex].GetId();
    if (ShaderId == EN_HANDLE_INVALID_ID) {
        return;
    }

    glDetachShader(MId, ShaderId);
    glDeleteShader(ShaderId);

    Stages[ShaderIndex].SetId(0);
}

int32_t CPipeline::GetUniformLocation(const char *UniformName)
{
    return glGetUniformLocation(MId, UniformName);
}

void CPipeline::SetUniform(CTexture &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1i(UniformLocation, Value.GetUnit());
}

void CPipeline::SetUniform(float Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1f(UniformLocation, Value);
}

void CPipeline::SetUniform(int32_t Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1i(UniformLocation, Value);
}

void CPipeline::SetUniform(uint32_t Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1ui(UniformLocation, Value);
}

void CPipeline::SetUniform(glm::vec2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::vec3 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::vec4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::ivec2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::ivec3 &Value, const char *UniformName)
{ 
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::ivec4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::uvec2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::uvec3 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::uvec4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat2x3 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2x3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat2x4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2x4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat3x2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3x2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat3 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat3x4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3x4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat4x2 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4x2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat4x3 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4x3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CPipeline::SetUniform(glm::mat4 &Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}


void CPipeline::Unbind()
{
    glUseProgram(0);
}   

void CPipeline::Destroy()
{
    uint32_t ShaderId = 0;
    for (CHandle ShaderHandle : Stages) {
        ShaderId = ShaderHandle.GetId();
        if (ShaderId == EN_HANDLE_INVALID_ID) continue;

        glDetachShader(MId, ShaderId);
        glDeleteShader(ShaderId);

        ShaderHandle.SetId(0);
    }
    
    glDeleteProgram(MId);
}
