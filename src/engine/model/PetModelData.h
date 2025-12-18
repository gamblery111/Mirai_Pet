#pragma once

#include <vector>
#include <string>

namespace miraipet::Model
{
    /**
     * @brief 宠物模型数据类，用于存储3D模型的各项数据
     */
    class PetModelData
    {
    public:
        PetModelData();
        ~PetModelData();

        void SetVertices(const std::vector<float> &vertices);
        void SetNormals(const std::vector<float> &normals);
        void SetUVs(const std::vector<float> &uvs);
        void SetIndices(const std::vector<unsigned int> &indices);
        void SetTextures(const std::vector<std::string> &textures);

        const std::vector<float> &GetVertices() const;
        const std::vector<float> &GetNormals() const;
        const std::vector<float> &GetUVs() const;
        const std::vector<unsigned int> &GetIndices() const;
        const std::vector<std::string> &GetTextures() const;

    private:
        std::vector<float> m_vertices;       // 存储模型顶点数据的私有成员变量
        std::vector<float> m_normals;        // 存储模型法线数据的私有成员变量
        std::vector<float> m_uvs;            // 存储模型UV坐标数据的私有成员变量
        std::vector<unsigned int> m_indices; // 存储模型索引数据的私有成员变量
        std::vector<std::string> m_textures; // 存储模型纹理路径的私有成员变量
    };
}