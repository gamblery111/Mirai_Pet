#pragma once

#include "GLObject.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace miraipet::OpenGL
{
    GLProgramObject CreateShaderProgram(const char *vsCode, const char *fsCode);

    void SetUniform(GLint uniform, GLint value);
    void SetUniform(GLint uniform, float value);
    void SetUniform(GLint uniform, const glm::vec2 &value);
    void SetUniform(GLint uniform, const glm::vec3 &value);
    void SetUniform(GLint uniform, const glm::vec4 &value);
    void SetUniform(GLint uniform, const glm::mat3 &value);
    void SetUniform(GLint uniform, const glm::mat4 &value);
    void SetUniform(GLint uniform, const GLint *values, GLsizei count);
    void SetUniform(GLint uniform, const float *values, GLsizei count);
    void SetUniform(GLint uniform, const glm::mat4 *values, GLsizei count);
}