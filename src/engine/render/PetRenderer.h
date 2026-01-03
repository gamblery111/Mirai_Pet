#pragma once


#include <memory>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

// #include "glm/glm.hpp"

namespace miraipet::context
{
    struct RenderContext;
}

namespace miraipet::render
{

    class PetRenderer
    {
        using RenderCtx = context::RenderContext;
    private:
        RenderCtx *m_ctx;
        QOpenGLShaderProgram *m_program{nullptr};
        QOpenGLVertexArrayObject m_vao;
        QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer m_ibo{QOpenGLBuffer::IndexBuffer};
        std::unique_ptr<QOpenGLTexture> m_texture;
        std::unique_ptr<QOpenGLTexture> m_defaultTexture;
        GLsizei m_indexCount = 0;
        unsigned int m_indexType = GL_UNSIGNED_INT; // 索引类型，支持GL_UNSIGNED_SHORT和GL_UNSIGNED_INT
        int m_viewportWidth = 0;
        int m_viewportHeight = 0;

        // glm::mat4 m_proj;

    public:
        PetRenderer(RenderCtx *ctx);
        ~PetRenderer();

        void Render();
        void ResizeGL(int w, int h);
        void UpdateModelData();

    private:
        void Initialize();

        /**
         * @brief 清理资源
         */
        void Cleanup();

        /**
         * @brief 设置顶点缓冲区和索引缓冲区
         */
        void SetupModelBuffers();

        /**
         * @brief 加载模型纹理
         */
        void LoadModelTexture();

        /**
         * @brief 创建默认纹理
         */
        void CreateDefaultTexture();
    };

}
