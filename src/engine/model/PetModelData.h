#pragma once

#include <vector>
#include <string>

namespace miraipet::Model
{
    struct Vertex
    {
        struct Position {
            float x, y, z;
        } position; // 顶点位置

        struct Normal {
            float x, y, z;
        } normal;   // 顶点法线

        struct TexCoord {
            float u, v;
        } texCoord; // 纹理坐标
    };

    /**
     * @brief 宠物模型数据类，用于存储3D模型的各项数据
     */
    class PetModelData
    {
    public:
        PetModelData();
        ~PetModelData();

        void SetVertices(const std::vector<Vertex> &vertices);
        void SetIndices(const std::vector<unsigned int> &indices);
        void SetTextures(const std::vector<std::string> &textures);

        const std::vector<Vertex> &GetVertices() const;
        const std::vector<unsigned int> &GetIndices() const;
        const std::vector<std::string> &GetTextures() const;

    private:
        std::vector<Vertex> m_vertices;       // 存储模型顶点数据的私有成员变量
        std::vector<unsigned int> m_indices; // 存储模型索引数据的私有成员变量
        std::vector<std::string> m_textures; // 存储模型纹理路径的私有成员变量
    };
}