#pragma once

#include "GLFacade.h"

#include <qopenglfunctions.h>

namespace miraipet::OpenGL
{

    template <GLenum ShaderType>
    struct GLShaderTraits
    {
        static GLuint Create()
        {
            return GL::CreateShader(ShaderType);
        }

        static void Destroy(GLuint shader)
        {
            GL::DeleteShader(shader);
        }
    };

    struct GLTextureTraits
    {
        static GLuint Create()
        {
            GLuint tex;
            glGenTextures(1, &tex);
            return tex;
        }

        static void Destroy(GLuint tex)
        {
            glDeleteTextures(1, &tex);
        }
    };

    struct GLFramebufferTraits
    {
        static GLuint Create()
        {
            GLuint fb;
            GL::GenFramebuffers(1, &fb);
            return fb;
        }

        static void Destroy(GLuint fb)
        {
            GL::DeleteFramebuffers(1, &fb);
        }
    };

    // Traits for OpenGL Program objects
    struct GLProgramTraits
    {
        static GLuint Create()
        {
            return GL::CreateProgram();
        }

        static void Destroy(GLuint program)
        {
            GL::DeleteProgram(program);
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

        explicit operator GLuint() const { return m_obj; }
        explicit operator bool() const { return m_obj != 0; }
    };

    template <typename GLTraits>
    class GLRef
    {
    private:
        class GLRefCount
        {
        private:
            std::atomic_int m_nCounter;
            GLint m_obj;

        public:
            GLRefCount(GLint obj)
                : m_nCounter(1), m_obj(obj)
            {
            }
            GLRefCount(const GLRefCount &) = delete;
            GLRefCount &operator=(const GLRefCount &) = delete;

            void IncRef()
            {
                m_nCounter.fetch_add(1);
            }

            void DecRef()
            {
                if (--m_nCounter == 0)
                {
                    if (m_obj != 0)
                    {
                        GLTraits::Destroy(m_obj);
                        m_obj = 0;
                        delete this;
                    }
                }
            }
            GLuint Get() const
            {
                return m_obj;
            }

            operator GLuint() const { return m_obj; }

            int GetCount() const { return m_nCounter; }
        };

    private:
        GLRefCount *m_ref;

    public:
        using Type = GLTraits;
        using RefType = GLRef<GLTraits>;
        GLRef()
            : m_ref(nullptr)
        {
        }

        ~GLRef()
        {
            if (m_ref)
            {
                m_ref->DecRef();
            }
        }

        GLRef(GLuint obj)
            : m_ref(nullptr)
        {
            Reset(obj);
        }

        GLRef(RefType &&rhs)
            : m_ref(nullptr)
        {
            Reset(rhs.m_ref);
            rhs.Reset();
        }

        GLRef(GLObject<GLTraits> &&rhs)
            : m_ref(nullptr)
        {
            Reset(rhs.Release());
        }

        RefType &operator=(RefType &&rhs)
        {
            Reset(rhs.m_ref);
            rhs.Reset();
            return *this;
        }

        RefType &operator=(GLObject<GLTraits> &&rhs)
        {
            Reset(rhs.Release());
            return *this;
        }

        GLRef(const RefType &rhs)
            : m_ref(nullptr)
        {
            Reset(const_cast<GLRefCount *>(rhs.m_ref));
        }

        RefType &operator=(const RefType &rhs)
        {
            Reset(const_cast<GLRefCount *>(rhs.m_ref));
            return *this;
        }

        GLuint Get() const
        {
            if (m_ref != nullptr)
            {
                return m_ref->Get();
            }
            return 0;
        }

        operator GLuint() const
        {
            if (m_ref != nullptr)
            {
                return m_ref->Get();
            }
            else
            {
                return 0;
            }
        }

        int GetRefCount() const
        {
            if (m_ref != nullptr)
            {
                return m_ref->GetCount();
            }
            else
            {
                return 0;
            }
        }

        void Release()
        {
            Reset();
        }

    private:
        void Reset()
        {
            if (m_ref != nullptr)
            {
                m_ref->DecRef();
                m_ref = nullptr;
            }
        }

        void Reset(GLuint obj)
        {
            Reset();
            if (obj != 0)
            {
                m_ref = new GLRefCount(obj);
            }
        }

        void Reset(GLRefCount *refobj)
        {
            Reset();
            if (refobj != nullptr)
            {
                m_ref = refobj;
                m_ref->IncRef();
            }
        }
    };

    template <GLenum ShaderType>
    using GLShaderObject = GLObject<GLShaderTraits<ShaderType>>; ///< 着色器对象

    using GLVertexShaderObject = GLShaderObject<GL_VERTEX_SHADER>;     ///< 顶点着色器对象
    using GLFragmentShaderObject = GLShaderObject<GL_FRAGMENT_SHADER>; ///< 片元着色器对象
    using GLTextureObject = GLObject<GLTextureTraits>;                 ///< 纹理对象
    using GLProgramObject = GLObject<GLProgramTraits>;                 ///< 程序对象
    using GLFramebufferObject = GLObject<GLFramebufferTraits>;         ///< 帧缓冲区对象

    template <GLenum ShaderType>
    using GLShaderRef = GLRef<GLShaderTraits<ShaderType>>; ///< 着色器引用对象
    using GLTextureRef = GLRef<GLTextureTraits>;           ///< 纹理引用对象
}
