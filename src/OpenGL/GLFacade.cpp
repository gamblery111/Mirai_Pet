#include "GLFacade.h"

#include <cassert>

#include <QOpenGLFunctions>

// ================= Internal Interface (hidden from business) =================
class IGL
{
public:
    virtual ~IGL() = default;
    virtual GLuint CreateShader(GLenum type) = 0;
    virtual void DeleteShader(GLuint id) = 0;
    virtual void ShaderSource(GLuint shader, const char *src) = 0;
    virtual void CompileShader(GLuint shader) = 0;
    virtual GLuint CreateProgram() = 0;
    virtual void DeleteProgram(GLuint id) = 0;
    virtual void AttachShader(GLuint program, GLuint shader) = 0;
    virtual void LinkProgram(GLuint program) = 0;
    virtual void GetProgramiv(GLuint program, GLenum pname, GLint *params) = 0;
    virtual void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog) = 0;
    virtual void GetShaderiv(GLuint shader, GLenum pname, GLint *params) = 0;
    virtual void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) = 0;

    // 新增函数声明
    virtual GLint GetUniformLocation(GLuint program, const char *name) = 0;
    virtual GLint GetAttribLocation(GLuint program, const char *name) = 0;

    virtual void Uniform1i(GLint location, GLint v0) = 0;
    virtual void Uniform1f(GLint location, GLfloat v0) = 0;
    virtual void Uniform2fv(GLint location, GLsizei count, const GLfloat *value) = 0;
    virtual void Uniform3fv(GLint location, GLsizei count, const GLfloat *value) = 0;
    virtual void Uniform4fv(GLint location, GLsizei count, const GLfloat *value) = 0;
    virtual void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;
    virtual void Uniform1iv(GLint location, GLsizei count, const GLint *value) = 0;
    virtual void Uniform1fv(GLint location, GLsizei count, const GLfloat *value) = 0;
};

// ================= Qt Implementation =================
class QtGL : public IGL
{
public:
    QOpenGLFunctions *f() const
    {
        auto ctx = QOpenGLContext::currentContext();
        assert(ctx);
        return ctx->functions();
    }

    GLuint CreateShader(GLenum type) override { return f()->glCreateShader(type); }
    void DeleteShader(GLuint id) override { f()->glDeleteShader(id); }
    void ShaderSource(GLuint shader, const char *src) override
    {
        f()->glShaderSource(shader, 1, &src, nullptr);
    }
    void CompileShader(GLuint shader) override { f()->glCompileShader(shader); }
    GLuint CreateProgram() override { return f()->glCreateProgram(); }
    void DeleteProgram(GLuint id) override { f()->glDeleteProgram(id); }
    void AttachShader(GLuint program, GLuint shader) override { f()->glAttachShader(program, shader); }
    void LinkProgram(GLuint program) override { f()->glLinkProgram(program); }
    void GetProgramiv(GLuint program, GLenum pname, GLint *params) override
    {
        f()->glGetProgramiv(program, pname, params);
    }
    void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog) override
    {
        f()->glGetProgramInfoLog(program, bufSize, length, infoLog);
    }
    void GetShaderiv(GLuint shader, GLenum pname, GLint *params) override
    {
        f()->glGetShaderiv(shader, pname, params);
    }
    void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) override
    {
        f()->glGetShaderInfoLog(shader, bufSize, length, infoLog);
    }

    // 新增函数实现
    GLint GetUniformLocation(GLuint program, const char *name) override { return f()->glGetUniformLocation(program, name); }
    GLint GetAttribLocation(GLuint program, const char *name) override { return f()->glGetAttribLocation(program, name); }

    void Uniform1i(GLint location, GLint v0) override { f()->glUniform1i(location, v0); }
    void Uniform1f(GLint location, GLfloat v0) override { f()->glUniform1f(location, v0); }
    void Uniform2fv(GLint location, GLsizei count, const GLfloat *value) override { f()->glUniform2fv(location, count, value); }
    void Uniform3fv(GLint location, GLsizei count, const GLfloat *value) override { f()->glUniform3fv(location, count, value); }
    void Uniform4fv(GLint location, GLsizei count, const GLfloat *value) override { f()->glUniform4fv(location, count, value); }
    void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) override
    {
        f()->glUniformMatrix3fv(location, count, transpose, value);
    }
    void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) override
    {
        f()->glUniformMatrix4fv(location, count, transpose, value);
    }
    void Uniform1iv(GLint location, GLsizei count, const GLint *value) override { f()->glUniform1iv(location, count, value); }
    void Uniform1fv(GLint location, GLsizei count, const GLfloat *value) override { f()->glUniform1fv(location, count, value); }
};

// ================= GL Facade Implementation =================
static IGL *g_impl = nullptr;

namespace miraipet::GL
{
    void Init(GLBackend backend)
    {
        if (g_impl)
            return; // already initialized

        switch (backend)
        {
        case GLBackend::Qt:
            g_impl = new QtGL();
            break;
        default:
            assert(false && "Unsupported GL backend");
            break;
        }
    }

    GLuint GL::CreateShader(GLenum type)
    {
        assert(g_impl && "GL not initialized!");
        return g_impl->CreateShader(type);
    }

    void GL::DeleteShader(GLuint id)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->DeleteShader(id);
    }

    void GL::ShaderSource(GLuint shader, const char *src)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->ShaderSource(shader, src);
    }

    void GL::CompileShader(GLuint shader)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->CompileShader(shader);
    }

    GLuint CreateProgram()
    {
        assert(g_impl && "GL not initialized!");
        return g_impl->CreateProgram();
    }

    void DeleteProgram(GLuint id)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->DeleteProgram(id);
    }

    void AttachShader(GLuint program, GLuint shader)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->AttachShader(program, shader);
    }

    void LinkProgram(GLuint program)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->LinkProgram(program);
    }

    void GetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->GetProgramiv(program, pname, params);
    }

    void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->GetProgramInfoLog(program, bufSize, length, infoLog);
    }

    void GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->GetShaderiv(shader, pname, params);
    }

    void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->GetShaderInfoLog(shader, bufSize, length, infoLog);
    }

    // 新增函数实现
    GLint GetUniformLocation(GLuint program, const char *name)
    {
        assert(g_impl && "GL not initialized!");
        return g_impl->GetUniformLocation(program, name);
    }

    GLint GetAttribLocation(GLuint program, const char *name)
    {
        assert(g_impl && "GL not initialized!");
        return g_impl->GetAttribLocation(program, name);
    }

    void Uniform1i(GLint location, GLint v0)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform1i(location, v0);
    }

    void Uniform1f(GLint location, GLfloat v0)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform1f(location, v0);
    }

    void Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform2fv(location, count, value);
    }

    void Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform3fv(location, count, value);
    }

    void Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform4fv(location, count, value);
    }

    void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->UniformMatrix3fv(location, count, transpose, value);
    }

    void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->UniformMatrix4fv(location, count, transpose, value);
    }

    void Uniform1iv(GLint location, GLsizei count, const GLint *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform1iv(location, count, value);
    }

    void Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
    {
        assert(g_impl && "GL not initialized!");
        g_impl->Uniform1fv(location, count, value);
    }

}