#include "PetRenderer.h"
#include "PetShaderManager.h"

namespace miraipet::render
{

    PetRenderer::PetRenderer(RenderCtx *ctx)
        : m_ctx(ctx)
    {
        Initialize();
    }

    PetRenderer::~PetRenderer()
    {
        if (m_ctx && m_ctx->gl)
        {
            m_ctx->gl->glDeleteBuffers(1, &m_vbo);
            m_ctx->gl->glDeleteVertexArrays(1, &m_vao);
        }
        delete m_program;
    }

    void PetRenderer::Render()
    {
        m_ctx->gl->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        m_ctx->gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->bind();
        m_ctx->gl->glBindVertexArray(m_vao);
        m_ctx->gl->glDrawArrays(GL_TRIANGLES, 0, 3);
        m_ctx->gl->glBindVertexArray(0);
        m_program->release();
    }

    void PetRenderer::Initialize()
    {
        m_program = m_ctx->shaderManager->CreateBasicShader();
        CreateTriangle();
    }

    void PetRenderer::CreateTriangle()
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};

        m_ctx->gl->glGenVertexArrays(1, &m_vao);
        m_ctx->gl->glGenBuffers(1, &m_vbo);

        m_ctx->gl->glBindVertexArray(m_vao);
        m_ctx->gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        m_ctx->gl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        m_ctx->gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        m_ctx->gl->glEnableVertexAttribArray(0);

        m_ctx->gl->glBindVertexArray(0);
    }
}