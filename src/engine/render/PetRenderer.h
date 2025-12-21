#pragma once

#include <QOpenGLShaderProgram>

namespace miraipet::context{struct RenderContext;}

namespace miraipet::render
{

    class PetRenderer
    {
        using RenderCtx = context::RenderContext;

    private:
        RenderCtx *m_ctx;
        QOpenGLShaderProgram *m_program{nullptr};
        unsigned int m_vao{0};
        unsigned int m_vbo{0};
        unsigned int m_ibo{0};
        unsigned int m_texture{0};
        GLsizei m_indexCount = 0;
        unsigned int m_indexType = GL_UNSIGNED_INT; // 索引类型，支持GL_UNSIGNED_SHORT和GL_UNSIGNED_INT


    public:
        PetRenderer(RenderCtx *ctx);
        ~PetRenderer();

        void Render();
        void UpdateModelData();

    private:
        void Initialize();

        /**
         * @brief 清理资源
         */
        void Cleanup();
        void SetupModelBuffers();
        void LoadModelTexture();
    };

}