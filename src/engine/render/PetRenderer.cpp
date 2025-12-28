#include <QImage>
#include <QVector4D>

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
        f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!m_vao.isCreated())
            return;

        m_program->bind();

        // 设置矩阵变换
        QMatrix4x4 model, view, projection;
        model.setToIdentity();
        view.translate(0, 0, -5.0f);
        projection.perspective(45.0f, m_ctx->aspectRatio, 0.1f, 100.0f);

        QMatrix4x4 modelView = view * model;
        QMatrix4x4 mvp = projection * modelView;
        QMatrix3x3 normalMatrix = modelView.normalMatrix();

        m_program->setUniformValue("uMVP", mvp);
        m_program->setUniformValue("uMV", modelView);
        m_program->setUniformValue("uN", normalMatrix);

        // 设置材质参数
        m_program->setUniformValue("uDiff", QVector4D(1.0f, 1.0f, 1.0f, 1.0f)); // 默认白色漫反射
        m_program->setUniformValue("uAlpha", 1.0f); // 默认不透明

        // 绑定主纹理（模型纹理或默认纹理）
        QOpenGLTexture *textureToBind = m_texture ? m_texture.get() : m_defaultTexture.get();
        textureToBind->bind(0);
        m_program->setUniformValue("uTex", 0);

        // 绑定toon纹理
        if (m_ctx->shaderManager->GetToonTexture()) {
            m_ctx->shaderManager->GetToonTexture()->bind(1);
            m_program->setUniformValue("uToon", 1);
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
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        f->initializeOpenGLFunctions();

        // 设置OpenGL状态
        f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        f->glEnable(GL_DEPTH_TEST);
        f->glEnable(GL_BLEND);
        f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_program = m_ctx->shaderManager->GetMMDShader();
        m_vao.create();
        m_vbo.create();
        m_ibo.create();
        m_texture.reset();
        CreateDefaultTexture();
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

    void PetRenderer::CreateDefaultTexture()
    {
        // 创建1x1白色像素的默认纹理
        QImage defaultImage(1, 1, QImage::Format_RGBA8888);
        defaultImage.setPixelColor(0, 0, QColor(255, 255, 255, 255));

        m_defaultTexture = std::make_unique<QOpenGLTexture>(defaultImage);
        m_defaultTexture->setMinificationFilter(QOpenGLTexture::Linear);
        m_defaultTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        m_defaultTexture->setWrapMode(QOpenGLTexture::Repeat);
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