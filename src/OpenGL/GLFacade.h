#pragma once

#include <windows.h>
#include <GL/GL.h>

namespace miraipet::GL
{
    enum class GLBackend
    {
        Qt
    };

    void Init(GLBackend backend);

    GLuint CreateShader(GLenum type);
    void DeleteShader(GLuint id);
    void ShaderSource(GLuint shader, const char *src);
    void CompileShader(GLuint shader);

    GLuint CreateProgram();
    void DeleteProgram(GLuint id);

    void AttachShader(GLuint program, GLuint shader);
    void LinkProgram(GLuint program);
    void GetProgramiv(GLuint program, GLenum pname, GLint *params);
    void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
    void GetShaderiv(GLuint shader, GLenum pname, GLint *params);
    void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog);


    void Uniform1i(GLint location, GLint v0);
    void Uniform1f(GLint location, GLfloat v0);
    void Uniform2fv(GLint location, GLsizei count, const GLfloat *value);
    void Uniform3fv(GLint location, GLsizei count, const GLfloat *value);
    void Uniform4fv(GLint location, GLsizei count, const GLfloat *value);
    void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void Uniform1iv(GLint location, GLsizei count, const GLint *value);
    void Uniform1fv(GLint location, GLsizei count, const GLfloat *value);
}