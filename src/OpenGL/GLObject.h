#pragma once

#include <QOpenGLFunctions>

namespace miraipet::OpenGL
{
    template <GLenum ShaderType>
    struct GLShaderTraits
    {
        static GLuint Create()
        {
            auto f = QOpenGLContext::currentContext()->functions();
            return f->glCreateShader(ShaderType);
        }

        static void Destroy(GLuint shader)
        {
            auto f = QOpenGLContext::currentContext()->functions();
            f->glDeleteShader(shader);
        }
    };

    // Traits for OpenGL Program objects
    struct GLProgramTraits
    {
        static GLuint Create()
        {
            auto f = QOpenGLContext::currentContext()->functions();
            return f->glCreateProgram();
        }

        static void Destroy(GLuint program)
        {
            auto f = QOpenGLContext::currentContext()->functions();
            f->glDeleteProgram(program);
        }
    };

    template <typename GLTraits>
    class GLObject
    {
    private:
        GLuint m_obj;

    public:
        using GLObjectType = GLObject<GLTraits>;

        GLObject()
            : m_obj(0)
        {
        }

        explicit GLObject(GLuint obj)
            : m_obj(obj)
        {
        }

        GLObject(GLObjectType &&rhs)
        {
            m_obj = rhs.m_obj;
            rhs.m_obj = 0;
        }

        GLObjectType &operator=(GLObjectType &&rhs)
        {
            m_obj = rhs.m_obj;
            rhs.m_obj = 0;
            return *this;
        }

        GLObject(const GLObjectType &rhs) = delete;
        GLObjectType &operator=(const GLObjectType &rhs) = delete;

        ~GLObject()
        {
            Destroy();
        }

        bool Create()
        {
            Destroy();
            m_obj = GLTraits::Create();
            return m_obj != 0;
        }

        void Destroy()
        {
            if (m_obj != 0)
            {
                GLTraits::Destroy(m_obj);
                m_obj = 0;
            }
        }

        void Reset(GLuint obj)
        {
            Destroy();
            m_obj = obj;
        }

        GLuint Release()
        {
            GLuint ret = m_obj;
            m_obj = 0;
            return ret;
        }

        GLuint Get() const
        {
            return m_obj;
        }

        operator GLuint() const { return m_obj; }
    };

    template <GLenum ShaderType>
    using GLShaderObject = GLObject<GLShaderTraits<ShaderType>>;

    using GLVertexShaderObject = GLShaderObject<GL_VERTEX_SHADER>;
    using GLFragmentShaderObject = GLShaderObject<GL_FRAGMENT_SHADER>;

    using GLProgramObject = GLObject<GLProgramTraits>;
}
