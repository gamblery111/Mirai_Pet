#include <QImage>

#include "PetRenderer.h"
#include "PetShaderManager.h"
#include "context/RenderContext.h"
#include "model/PetModelData.h"

#include <QOpenGLFunctions>

#include <glm/gtc/matrix_transform.hpp>

namespace miraipet::render
{

    PetRenderer::PetRenderer(RenderCtx *ctx)
        : m_ctx(ctx)
    {
        Initialize();
    }

    PetRenderer::~PetRenderer()
    {
        Cleanup();
        delete m_program;
    }

    void PetRenderer::Cleanup()
    {
        m_vao.destroy();
        m_vbo.destroy();
        m_ibo.destroy();
        if (m_texture)
            m_texture.reset();
    }

    void PetRenderer::UpdateModelData()
    {
        SetupModelBuffers();
    }

    void PetRenderer::Render()
    {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!m_vao.isCreated())
            return;

        m_program->bind();

        // 设置矩阵变换
        QMatrix4x4 model, view, projection;
        model.setToIdentity();
        view.translate(0, 0, -5.0f);
        projection.perspective(45.0f, m_ctx->aspectRatio, 0.1f, 100.0f);

        m_program->setUniformValue("model", model);
        m_program->setUniformValue("view", view);
        m_program->setUniformValue("projection", projection);

        // 绑定纹理
        if (m_texture)
        {
            m_texture->bind(0);
            m_program->setUniformValue("texture1", 0);
        }

        // 渲染模型
        m_vao.bind();
        f->glDrawElements(GL_TRIANGLES, m_indexCount, m_indexType, nullptr);
        m_vao.release();

        m_program->release();
    }

    void PetRenderer::ResizeGL(int w, int h)
    {
        // m_proj = glm::perspective(glm::radians(45.0f), float(w) / qMax(h, 1), 0.1f, 1000.0f);
    }

    void PetRenderer::Initialize()
    {
        m_program = m_ctx->shaderManager->CreateMMDShader();
        m_vao.create();
        m_vbo.create();
        m_ibo.create();
        m_texture.reset();
    }

    void PetRenderer::SetupModelBuffers()
    {
        if (!m_ctx->currentModelData)
            return;

        // 清理旧的缓冲区
        Cleanup();

        // 获取顶点数据
        const auto &modelVertices = m_ctx->currentModelData->GetVertices();
        std::vector<float> vertices;

        for (const auto &vertex : modelVertices)
        {
            vertices.push_back(vertex.position[0]);
            vertices.push_back(vertex.position[1]);
            vertices.push_back(vertex.position[2]);
            vertices.push_back(vertex.normal[0]);
            vertices.push_back(vertex.normal[1]);
            vertices.push_back(vertex.normal[2]);
            vertices.push_back(vertex.uv[0]);
            vertices.push_back(vertex.uv[1]);
            for (int i = 0; i < 4; i++)
                vertices.push_back(static_cast<float>(vertex.boneIndices[i]));
            for (int i = 0; i < 4; i++)
                vertices.push_back(vertex.boneWeights[i]);
        }

        const unsigned char *indexData = m_ctx->currentModelData->GetIndexBuffer();
        m_indexCount = static_cast<GLsizei>(m_ctx->currentModelData->GetIndexCount());
        size_t indexElementSize = m_ctx->currentModelData->GetIndexElementSize();

        m_vao.create();
        m_vao.bind();

        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(vertices.data(), vertices.size() * sizeof(float));

        m_ibo.create();
        m_ibo.bind();
        m_ibo.allocate(indexData, m_indexCount * indexElementSize);

        m_indexType = (indexElementSize == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
        const int stride = 16 * sizeof(float);

        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        f->glEnableVertexAttribArray(2);
        f->glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void *)(8 * sizeof(float)));
        f->glEnableVertexAttribArray(3);
        f->glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void *)(12 * sizeof(float)));
        f->glEnableVertexAttribArray(4);

        m_vao.release();
        m_vbo.release();
        m_ibo.release();

        LoadModelTexture();
    }

    void PetRenderer::LoadModelTexture()
    {
        if (!m_ctx->currentModelData || m_ctx->currentModelData->GetTextures().empty())
            return;

        const std::string &texturePath = m_ctx->currentModelData->GetTextures()[0];
        QImage textureImage(QString::fromStdString(texturePath));
        if (textureImage.isNull())
            return;
        textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888);

        m_texture = std::make_unique<QOpenGLTexture>(textureImage);
        m_texture->setMinificationFilter(QOpenGLTexture::Linear);
        m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
        m_texture->setWrapMode(QOpenGLTexture::Repeat);
    }
}