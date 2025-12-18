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

    void PetModelData::SetVertices(const std::vector<float>& vertices)
    {
        m_vertices = vertices;
    }

    void PetModelData::SetNormals(const std::vector<float>& normals)
    {
        m_normals = normals;
    }

    void PetModelData::SetUVs(const std::vector<float>& uvs)
    {
        m_uvs = uvs;
    }

    void PetModelData::SetIndices(const std::vector<unsigned int>& indices)
    {
        m_indices = indices;
    }

    void PetModelData::SetTextures(const std::vector<std::string>& textures)
    {
        m_textures = textures;
    }

    const std::vector<float>& PetModelData::GetVertices() const
    {
        return m_vertices;
    }

    const std::vector<float>& PetModelData::GetNormals() const
    {
        return m_normals;
    }

    const std::vector<float>& PetModelData::GetUVs() const
    {
        return m_uvs;
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