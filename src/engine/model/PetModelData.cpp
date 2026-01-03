#include "PetModelData.h"
#include <utility>
#include <algorithm>

namespace miraipet::Model
{
    PetModelData::PetModelData()
    {
    }

    PetModelData::~PetModelData()
    {
    }

    void PetModelData::SetVertices(const std::vector<Vertex> &vertices)
    {
        m_vertices = vertices;
    }

    const std::vector<Vertex> &PetModelData::GetVertices() const
    {
        return m_vertices;
    }

    void PetModelData::SetIndexBuffer(const void *data, size_t count, size_t elementSize)
    {
        m_indexCount = count;
        m_indexElementSize = elementSize;

        // 按原始字节大小拷贝
        m_indexBuffer.resize(count * elementSize);
        memcpy(m_indexBuffer.data(), data, count * elementSize);
    }

    const unsigned char *PetModelData::GetIndexBuffer() const
    {
       return m_indexBuffer.data();
    }

    size_t PetModelData::GetIndexCount() const
    {
        return m_indexCount;
    }

    size_t PetModelData::GetIndexElementSize() const
    {
        return m_indexElementSize;
    }

    void PetModelData::SetTextures(const std::vector<std::string> &textures)
    {
        m_textures = textures;
    }

    const std::vector<std::string> &PetModelData::GetTextures() const
    {
        return m_textures;
    }

    void PetModelData::CalculateBoundingBox()
    {
        if (m_vertices.empty())
            return;

        // 初始化边界框
        m_boundingBox = {FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};

        // 遍历所有顶点，计算边界框
        for (const auto &vertex : m_vertices)
        {
            // 更新最小值
            m_boundingBox[0] = std::min(m_boundingBox[0], vertex.position[0]); // minX
            m_boundingBox[1] = std::min(m_boundingBox[1], vertex.position[1]); // minY
            m_boundingBox[2] = std::min(m_boundingBox[2], vertex.position[2]); // minZ

            // 更新最大值
            m_boundingBox[3] = std::max(m_boundingBox[3], vertex.position[0]); // maxX
            m_boundingBox[4] = std::max(m_boundingBox[4], vertex.position[1]); // maxY
            m_boundingBox[5] = std::max(m_boundingBox[5], vertex.position[2]); // maxZ
        }
    }

    const std::array<float, 6> &PetModelData::GetBoundingBox() const
    {
        return m_boundingBox;
    }
}
