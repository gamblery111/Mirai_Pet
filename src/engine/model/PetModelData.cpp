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

    void PetModelData::SetVertices(const std::vector<Vertex>& vertices)
    {
        m_vertices = vertices;
    }

    void PetModelData::SetIndices(const std::vector<unsigned int>& indices)
    {
        m_indices = indices;
    }

    void PetModelData::SetTextures(const std::vector<std::string>& textures)
    {
        m_textures = textures;
    }

    const std::vector<Vertex>& PetModelData::GetVertices() const
    {
        return m_vertices;
    }

    const std::vector<unsigned int>& PetModelData::GetIndices() const
    {
        return m_indices;
    }

    const std::vector<std::string>& PetModelData::GetTextures() const
    {
        return m_textures;
    }
}