#include "Uniform.h"

GLint CUniform::GetUniformLocation(CShader *Shader, std::string Name) {
    return glGetUniformLocation(Shader->GetId(), Name.c_str());
}

template<>
void CUniform::SetUniform<float>(CShader *Shader, std::string Name, float Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform1f(location, Value);
}

template<>
void CUniform::SetUniform<int>(CShader *Shader, std::string Name, int Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform1i(location, Value);
}

template<>
void CUniform::SetUniform<unsigned int>(CShader *Shader, std::string Name, unsigned int Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform1ui(location, Value);
}

template<>
void CUniform::SetUniform<glm::vec2>(CShader *Shader, std::string Name, glm::vec2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform2fv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::vec3>(CShader *Shader, std::string Name, glm::vec3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform3fv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::vec4>(CShader *Shader, std::string Name, glm::vec4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform4fv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::ivec2>(CShader *Shader, std::string Name, glm::ivec2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform2iv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::ivec3>(CShader *Shader, std::string Name, glm::ivec3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform3iv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::ivec4>(CShader *Shader, std::string Name, glm::ivec4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform4iv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::uvec2>(CShader *Shader, std::string Name, glm::uvec2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform2uiv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::uvec3>(CShader *Shader, std::string Name, glm::uvec3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform3uiv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::uvec4>(CShader *Shader, std::string Name, glm::uvec4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniform4uiv(location, 1, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat2>(CShader *Shader, std::string Name, glm::mat2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat2x3>(CShader *Shader, std::string Name, glm::mat2x3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat2x4>(CShader *Shader, std::string Name, glm::mat2x4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat3x2>(CShader *Shader, std::string Name, glm::mat3x2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat3>(CShader *Shader, std::string Name, glm::mat3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat3x4>(CShader *Shader, std::string Name, glm::mat3x4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat4x2>(CShader *Shader, std::string Name, glm::mat4x2 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat4x3>(CShader *Shader, std::string Name, glm::mat4x3 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}

template<>
void CUniform::SetUniform<glm::mat4>(CShader *Shader, std::string Name, glm::mat4 Value) {
    GLint location = GetUniformLocation(Shader, Name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Value));
}