#include "Shader.h"

const char *CShader::MatchShaderTypeString(EShaderType Type)
{
    switch (Type) {
    case EShaderType::Vertex:                           return "Vertex Shader";
    case EShaderType::TessellationControl:              return "Tessellation Control Shader";
    case EShaderType::TessellationEvaluation:           return "Tessellation Evaluation Shader";
    case EShaderType::Geometry:                         return "Geometry Shader";
    case EShaderType::Fragment:                         return "Fragment Shader";
    case EShaderType::Compute:                          return "Compute Shader";
    default:                                            return "Unknown Shader Type";
    }
}

CShader::CShader()
{
    Id = glCreateProgram();
}

uint8_t CShader::Load(const char *Filepath, EShaderType Type)
{
    char *Content = read_file(Filepath);

    uint32_t ShaderId = Compile(Content, Type);

    if (!ShaderId) {
        free(Content);
        return 0;
    }

    glAttachShader(Id, ShaderId);
    glLinkProgram(Id);
    glValidateProgram(Id);

    glDeleteShader(ShaderId);

    free(Content);
    return 1;
}

uint32_t CShader::Compile(const char *Source, EShaderType Type)
{
    uint32_t ShaderId = glCreateShader((GLenum)Type);
    glShaderSource(ShaderId, 1, &Source, nullptr);
    glCompileShader(ShaderId);

    int32_t Result;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);
    if (!Result) {
        int32_t Length;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &Length);
        char *Message = (char *)malloc(Length * sizeof(char));
        glGetShaderInfoLog(ShaderId, Length, &Length, Message);
        printf(
            "Failed to compile %s shader!\n%s\n",
            MatchShaderTypeString(Type),
            Message
        );
        free(Message);
        glDeleteShader(ShaderId);
        return 0;
    }

    return ShaderId;
}

void CShader::Bind()
{
    glUseProgram(Id);
}

void CShader::Unbind()
{
    glUseProgram(0);
}

int32_t CShader::GetUniformLocation(const char *UniformName)
{
    return glGetUniformLocation(Id, UniformName);
}


void CShader::SetUniform(CTexture *Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);

    glUniform1i(UniformLocation, Value->GetUnit());
}

void CShader::SetUniform(float Value, const char *UniformName) {
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1f(UniformLocation, Value);
}

void CShader::SetUniform(int32_t Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1i(UniformLocation, Value);
}

void CShader::SetUniform(uint32_t Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform1ui(UniformLocation, Value);
}

void CShader::SetUniform(glm::vec2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::vec3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::vec4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4fv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::ivec2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::ivec3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::ivec4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4iv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::uvec2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform2uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::uvec3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform3uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::uvec4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniform4uiv(UniformLocation, 1, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat2x3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2x3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat2x4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix2x4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat3x2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3x2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat3x4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix3x4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat4x2 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4x2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat4x3 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4x3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

void CShader::SetUniform(glm::mat4 Value, const char *UniformName)
{
    int32_t UniformLocation = GetUniformLocation(UniformName);
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

CShader::~CShader()
{
    glDeleteShader(Id);
}