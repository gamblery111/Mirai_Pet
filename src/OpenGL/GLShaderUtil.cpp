#include <iostream>
#include <vector>

#include "GLShaderUtil.h"

#include <QOpenGLFunctions>

namespace miraipet::OpenGL
{
    namespace
    {
        const char *GetShaderString(GLenum shaderType)
        {
            switch (shaderType)
            {
            case GL_COMPUTE_SHADER:
                return "Compute Shader";
            case GL_VERTEX_SHADER:
                return "Vertex Shader";
            case GL_TESS_CONTROL_SHADER:
                return "Tess Control Shader";
            case GL_TESS_EVALUATION_SHADER:
                return "Tess Evaluation Shader";
            case GL_GEOMETRY_SHADER:
                return "Geometry Shader";
            case GL_FRAGMENT_SHADER:
                return "Fragment Shader";
            default:
                return "Unknown Shader Type";
            }
        }

        template <GLenum ShaderType>
        GLShaderObject<ShaderType> CreateShader(const char *code)
        {
            std::cout << "Create: " << GetShaderString(ShaderType) << "\n";
            GLShaderObject<ShaderType> shader;
            if (shader.Create() == 0)
            {
                return GLShaderObject<ShaderType>();
            }

            GL::ShaderSource((GLuint)shader, code);

            GL::CompileShader((GLuint)shader);

            GLint infoLength;
            GL::GetShaderiv((GLuint)shader, GL_INFO_LOG_LENGTH, &infoLength);
            if (infoLength != 0)
            {
                std::vector<char> info;
                info.reserve(size_t(infoLength) + 1);
                info.resize(infoLength);

                GLsizei len;
                GL::GetShaderInfoLog((GLuint)shader, infoLength, &len, &info[0]);
                if (info[size_t(infoLength) - 1] != '\0')
                {
                    info.push_back('\0');
                }

                std::cout << &info[0] << "\n";
            }

            GLint compileStatus;
            GL::GetShaderiv((GLuint)shader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus != GL_TRUE)
            {
                GL::DeleteShader((GLuint)shader);
                return GLShaderObject<ShaderType>();
            }

            std::cout << "Success: " << GetShaderString(ShaderType) << "\n";

            return shader;
        }
    }

    GLProgramObject CreateShaderProgram(const char *vsCode, const char *fsCode)
    {
        GLVertexShaderObject vs(CreateShader<GL_VERTEX_SHADER>(vsCode));
        if (!vs)
        {
            return GLProgramObject();
        }

        GLFragmentShaderObject fs(CreateShader<GL_FRAGMENT_SHADER>(fsCode));
        if (!fs)
        {
            return GLProgramObject();
        }

        GLProgramObject prog;
        prog.Create();

        std::cout << "Start: Shader Program Link\n";

        GL::AttachShader((GLuint)prog, (GLuint)vs);
        GL::AttachShader((GLuint)prog, (GLuint)fs);
        GL::LinkProgram((GLuint)prog);

        GLint infoLength;
        GL::GetProgramiv((GLuint)prog, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength != 0)
        {
            std::vector<char> info;
            info.reserve(size_t(infoLength) + 1);
            info.resize(infoLength);

            GLsizei len;
            GL::GetProgramInfoLog((GLuint)prog, infoLength, &len, &info[0]);
            if (info[size_t(infoLength) - 1] != '\0')
            {
                info.push_back('\0');
            }

            std::cout << &info[0] << "\n";
        }

        GLint linkStatus;
        GL::GetProgramiv((GLuint)prog, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE)
        {
            return GLProgramObject();
        }

        std::cout << "Success: Shader Program Link\n";

        return prog;
    }
    void SetUniform(GLint uniform, GLint value)
    {
        GL::Uniform1i(uniform, value);
    }

    void SetUniform(GLint uniform, float value)
    {
        GL::Uniform1f(uniform, value);
    }

    void SetUniform(GLint uniform, const glm::vec2 &value)
    {
        GL::Uniform2fv(uniform, 1, &value[0]);
    }

    void SetUniform(GLint uniform, const glm::vec3 &value)
    {
        GL::Uniform3fv(uniform, 1, &value[0]);
    }

    void SetUniform(GLint uniform, const glm::vec4 &value)
    {
        GL::Uniform4fv(uniform, 1, &value[0]);
    }

    void SetUniform(GLint uniform, const glm::mat3 &value)
    {
        GL::UniformMatrix3fv(uniform, 1, GL_FALSE, &value[0][0]);
    }

    void SetUniform(GLint uniform, const glm::mat4 &value)
    {
        GL::UniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
    }

    void SetUniform(GLint uniform, const GLint *values, GLsizei count)
    {
        GL::Uniform1iv(uniform, count, values);
    }

    void SetUniform(GLint uniform, const float *values, GLsizei count)
    {
        GL::Uniform1fv(uniform, count, values);
    }

    void SetUniform(GLint uniform, const glm::mat4 *values, GLsizei count)
    {
        GL::UniformMatrix4fv(uniform, count, GL_FALSE, &values[0][0][0]);
    }
}