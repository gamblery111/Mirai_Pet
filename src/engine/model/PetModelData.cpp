#include "PetModelData.h"
#include <utility>

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
}