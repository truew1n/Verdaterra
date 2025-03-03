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

CShader::~CShader()
{
    glDeleteShader(Id);
}