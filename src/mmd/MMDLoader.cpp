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
        const auto *positions = pmx.GetPositions();
        const auto *normals = pmx.GetNormals();
        // 获取顶点的纹理坐标（UV）数据
        const auto *texCoords = pmx.GetUVs();
        size_t vertexCount = pmx.GetVertexCount();

        for (size_t i = 0; i < vertexCount; i++)
        {
            Model::Vertex vertex{};
            vertex.position = {positions[i].x * m_fModelScale, positions[i].y * m_fModelScale, positions[i].z * m_fModelScale};
            vertex.normal = {normals[i].x, normals[i].y, normals[i].z};
            vertex.uv = {texCoords[i].x, texCoords[i].y};
            // 默认骨骼：全部设置 0
            // 如果你之后要解析 BDEF/SDEF，再填充骨骼与权重
            vertex.boneIndices[0] = 0;
            vertex.boneWeights[0] = 1.0f;

            for (int j = 1; j < 4; j++)
            {
                vertex.boneIndices[j] = 0;
                vertex.boneWeights[j] = 0.0f;
            }
            vertices.push_back(vertex);
        }
        modelData->SetVertices(vertices);

        // 设置索引数据
        const void *indexData = pmx.GetIndices();
        size_t indexCount = pmx.GetIndexCount();
        size_t indexElementSize = pmx.GetIndexElementSize();
        modelData->SetIndexBuffer(indexData, indexCount, indexElementSize);

        // 设置纹理数据
        std::vector<std::string> textures;
        // PMX模型不直接提供GetTextures方法，需要从材质中获取纹理路径
        size_t materialCount = pmx.GetMaterialCount();
        const saba::MMDMaterial *materials = pmx.GetMaterials();

        for (size_t i = 0; i < materialCount; i++)
        {
            // 从材质中获取纹理路径
            const std::string &texturePath = materials[i].m_texture;
            if (!texturePath.empty() && std::find(textures.begin(), textures.end(), texturePath) == textures.end())
            {
                textures.push_back(texturePath);
            }
        }
        modelData->SetTextures(textures);

        return modelData;
    }

}