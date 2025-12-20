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

    std::shared_ptr<Model::PetModelData> MMDLoader::load(const std::string &_pmxPath, const std::string &mmdDataDir)
    {
        saba::PMXModel pmx;
        if (!pmx.Load(_pmxPath, mmdDataDir))
        {
            throw std::runtime_error("Failed to load PMX model.");
        }

        std::shared_ptr<Model::PetModelData> modelData = std::make_shared<Model::PetModelData>();

        // 设置顶点数据
        std::vector<Model::Vertex> vertices;
        const auto* positions = pmx.GetPositions();
        const auto* normals = pmx.GetNormals();
        // 获取顶点的纹理坐标（UV）数据
        const auto* texCoords = pmx.GetUVs(); 
        size_t vertexCount = pmx.GetVertexCount();

        for (size_t i = 0; i < vertexCount; i++)
        {
            Model::Vertex vertex;
            vertex.position = {positions[i].x, positions[i].y, positions[i].z};
            vertex.normal = {normals[i].x, normals[i].y, normals[i].z};
            vertex.texCoord = {texCoords[i].x, texCoords[i].y};
            vertices.push_back(vertex);
        }
        modelData->SetVertices(vertices);

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
                indices.push_back(indexPtr[i]);
            }
        } else if (indexElementSize == 4) {
            // 32-bit indices
            const uint32_t* indexPtr = static_cast<const uint32_t*>(indexData);
            for (size_t i = 0; i < indexCount; i++) {
                indices.push_back(indexPtr[i]);
            }
        }
        modelData->SetIndices(indices);

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
        modelData->SetTextures(textures);

        return modelData;
    }

}