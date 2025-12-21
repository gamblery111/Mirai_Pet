#include <QImage>

#include "PetRenderer.h"
#include "PetShaderManager.h"
#include "context/RenderContext.h"
#include "model/PetModelData.h"

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
        if (m_ctx && m_ctx->gl)
        {
            if (m_vao != 0)
                m_ctx->gl->glDeleteVertexArrays(1, &m_vao);
            if (m_vbo != 0)
                m_ctx->gl->glDeleteBuffers(1, &m_vbo);
            if (m_ibo != 0)
                m_ctx->gl->glDeleteBuffers(1, &m_ibo);
            if (m_texture != 0)
                m_ctx->gl->glDeleteTextures(1, &m_texture);
        }
    }

    void PetRenderer::UpdateModelData()
    {
        SetupModelBuffers();
    }

    void PetRenderer::Render()
    {
        m_ctx->gl->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        m_ctx->gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_vao == 0)
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
        if (m_texture != 0)
        {
            m_ctx->gl->glActiveTexture(GL_TEXTURE0);
            m_ctx->gl->glBindTexture(GL_TEXTURE_2D, m_texture);
            m_program->setUniformValue("texture1", 0);
        }

        // 渲染模型
        m_ctx->gl->glBindVertexArray(m_vao);
        m_ctx->gl->glDrawElements(GL_TRIANGLES, m_indexCount, m_indexType, nullptr);
        m_ctx->gl->glBindVertexArray(0);

        m_program->release();
    }

    void PetRenderer::Initialize()
    {
        m_program = m_ctx->shaderManager->CreateMMDShader();

        // 初始化VAO, VBO, IBO为0，表示尚未创建
        m_vao = 0;
        m_vbo = 0;
        m_ibo = 0;
        m_texture = 0;
    }

    void PetRenderer::SetupModelBuffers()
    {
        if (!m_ctx->gl || !m_ctx->currentModelData)
            return;

        // 清理旧的缓冲区
        Cleanup();

        // 获取顶点数据
        const auto &modelVertices = m_ctx->currentModelData->GetVertices();
        std::vector<float> vertices;

        // 将顶点数据组合成一个连续的数组
        for (const auto &vertex : modelVertices)
        {
            // 位置 (x, y, z)
            vertices.push_back(vertex.position[0]);
            vertices.push_back(vertex.position[1]);
            vertices.push_back(vertex.position[2]);

            // 法线 (nx, ny, nz)
            vertices.push_back(vertex.normal[0]);
            vertices.push_back(vertex.normal[1]);
            vertices.push_back(vertex.normal[2]);

            // UV坐标 (u, v)
            vertices.push_back(vertex.uv[0]);
            vertices.push_back(vertex.uv[1]);

            // 骨骼索引 (4个int转换为4个float)
            for (int i = 0; i < 4; i++)
            {
                vertices.push_back(static_cast<float>(vertex.boneIndices[i]));
            }

            // 骨骼权重 (4个float)
            for (int i = 0; i < 4; i++)
            {
                vertices.push_back(vertex.boneWeights[i]);
            }
        }

        // 获取索引数据
        const unsigned char *indexData = m_ctx->currentModelData->GetIndexBuffer();
        m_indexCount = static_cast<GLsizei>(m_ctx->currentModelData->GetIndexCount());
        size_t indexElementSize = m_ctx->currentModelData->GetIndexElementSize();

        // 创建并填充VBO和IBO
        m_ctx->gl->glGenVertexArrays(1, &m_vao);
        m_ctx->gl->glGenBuffers(1, &m_vbo);
        m_ctx->gl->glGenBuffers(1, &m_ibo);

        m_ctx->gl->glBindVertexArray(m_vao);

        // VBO
        m_ctx->gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        m_ctx->gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // IBO - 根据索引元素大小设置不同类型
        m_ctx->gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        m_ctx->gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * indexElementSize, indexData, GL_STATIC_DRAW);

        // 根据索引元素大小设置绘制类型
        m_indexType = (indexElementSize == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

        // 设置顶点属性指针
        // 每个顶点16个float，步长为16 * sizeof(float)
        const int stride = 16 * sizeof(float);

        // 位置属性 (location = 0)
        m_ctx->gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        m_ctx->gl->glEnableVertexAttribArray(0);

        // 法线属性
        m_ctx->gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        m_ctx->gl->glEnableVertexAttribArray(1);

        // UV坐标属性
        m_ctx->gl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        m_ctx->gl->glEnableVertexAttribArray(2);

        // 骨骼索引属性
        m_ctx->gl->glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void *)(8 * sizeof(float)));
        m_ctx->gl->glEnableVertexAttribArray(3);

        // 骨骼权重属性
        m_ctx->gl->glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void *)(12 * sizeof(float)));
        m_ctx->gl->glEnableVertexAttribArray(4);

        // 解绑
        m_ctx->gl->glBindVertexArray(0);

        // 加载纹理
        LoadModelTexture();
    }

    void PetRenderer::LoadModelTexture()
    {
        if (!m_ctx->currentModelData || m_ctx->currentModelData->GetTextures().empty())
            return;

        // 加载第一个纹理作为演示
        // 实际应用中应该加载所有纹理并根据材质使用
        const std::string &texturePath = m_ctx->currentModelData->GetTextures()[0];

        // 使用Qt加载图像
        QImage textureImage(QString::fromStdString(texturePath));
        if (textureImage.isNull())
        {
            return;
        }

        // 转换为OpenGL兼容格式
        textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888);

        // 生成纹理
        m_ctx->gl->glGenTextures(1, &m_texture);
        m_ctx->gl->glBindTexture(GL_TEXTURE_2D, m_texture);

        m_ctx->gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());

        m_ctx->gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_ctx->gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_ctx->gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_ctx->gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        m_ctx->gl->glBindTexture(GL_TEXTURE_2D, 0);
    }
}