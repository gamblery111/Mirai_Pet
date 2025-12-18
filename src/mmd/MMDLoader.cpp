#include "MMDLoader.h"

#include "saba/Base/File.h"
#include "saba/Base/Path.h"
#include "saba/Model/MMD/PMXModel.h"

namespace miraipet::MMD
{

    MMDLoader::MMDLoader()
    {
    }

    MMDLoader::~MMDLoader()
    {
    }

    Model::PetModelData MMDLoader::load(const std::string &_pmxPath, const std::string &mmdDataDir)
    {
        saba::PMXModel pmx;
        if (!pmx.Load(_pmxPath, mmdDataDir))
        {
            throw std::runtime_error("Failed to load PMX model.");
        }

        Model::PetModelData modelData;

        // 设置顶点数据
        std::vector<float> vertices;
        const glm::vec3* positions = pmx.GetPositions();
        size_t vertexCount = pmx.GetVertexCount();
        for (size_t i = 0; i < vertexCount; i++)
        {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);
        }
        modelData.SetVertices(vertices);

        // 设置索引数据
        std::vector<unsigned int> indices;
        const void* indexData = pmx.GetIndices();
        size_t indexCount = pmx.GetIndexCount();
        size_t indexElementSize = pmx.GetIndexElementSize();

        indices.reserve(indexCount);

        if (indexElementSize == 2) {
            // 16-bit indices
            const uint16_t* indexPtr = static_cast<const uint16_t*>(indexData);
            for (size_t i = 0; i < indexCount; i++) {
                indices.push_back(static_cast<unsigned int>(indexPtr[i]));
            }
        } else if (indexElementSize == 4) {
            // 32-bit indices
            const uint32_t* indexPtr = static_cast<const uint32_t*>(indexData);
            for (size_t i = 0; i < indexCount; i++) {
                indices.push_back(static_cast<unsigned int>(indexPtr[i]));
            }
        }

        modelData.SetIndices(indices);

        // 设置法线数据
        std::vector<float> normals;
        const glm::vec3* normalData = pmx.GetNormals();
        for (size_t i = 0; i < vertexCount; i++)
        {
            normals.push_back(normalData[i].x);
            normals.push_back(normalData[i].y);
            normals.push_back(normalData[i].z);
        }
        modelData.SetNormals(normals);

        // 设置UV坐标数据
        std::vector<float> uvs;
        const glm::vec2* uvData = pmx.GetUVs();
        for (size_t i = 0; i < vertexCount; i++)
        {
            uvs.push_back(uvData[i].x);
            uvs.push_back(uvData[i].y);
        }
        modelData.SetUVs(uvs);

        // 设置纹理数据
        std::vector<std::string> textures;
        // PMX模型不直接提供GetTextures方法，需要从材质中获取纹理路径
        size_t materialCount = pmx.GetMaterialCount();
        const saba::MMDMaterial* materials = pmx.GetMaterials();

        for (size_t i = 0; i < materialCount; i++) {
            // 从材质中获取纹理路径
            const std::string& texturePath = materials[i].m_texture;
            if (!texturePath.empty() && std::find(textures.begin(), textures.end(), texturePath) == textures.end()) {
                textures.push_back(texturePath);
            }
        }
        modelData.SetTextures(textures);

        return modelData;
    }

}