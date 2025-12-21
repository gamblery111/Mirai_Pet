#pragma once

#include <vector>
#include <string>
#include <array>

namespace miraipet::Model
{
    /**
     * @brief 模型顶点结构（适配 PMX/MMD 基础骨骼蒙皮）
     *
     * 包含：
     * - 位置
     * - 法线
     * - UV
     * - 骨骼索引（最多4个）
     * - 骨骼权重（最多4个）
     *
     * 注意：
     * MMD 顶点支持 BDEF1/BDEF2/BDEF4/SDEF
     * 本结构覆盖最常见的 BDEF1/2/4
     */
    struct Vertex
    {
        // 顶点位置
        std::array<float, 3> position{0.0f};

        // 顶点法线
        std::array<float, 3> normal{0.0f};

        // 纹理 UV 坐标
        std::array<float, 2> uv{0.0f};

        // 骨骼索引（最多4个）
        // 用于 skinning，告诉 shader 该顶点由哪些骨骼控制
        std::array<int, 4> boneIndices{0};

        // 骨骼权重（与 boneIndices 对应）
        // 权重之和一般为 1
        std::array<float, 4> boneWeights{0.0f};
    };

    /**
     * @brief   用于存储一个 3D 模型的数据：
     *
     * - 顶点数据
     * - 索引数据（支持16/32位）
     * - 纹理路径
     *
     * OpenGL 渲染阶段将从此类中读取：
     * - vertex buffer
     * - index buffer
     * - texture list
     */
    class PetModelData
    {
    public:
        PetModelData();
        ~PetModelData();

        /**
         * @brief 顶点数据设置
         * @param vertices
         */
        void SetVertices(const std::vector<Vertex> &vertices);
        const std::vector<Vertex> &GetVertices() const;

        /**
         * @brief 索引数据设置
         *
         * @param data          PMX 解析出的索引原始数据指针
         * @param count         索引数量
         * @param elementSize   每个索引占用的字节大小 2 = uint16 , 4 = uint32
         * 注意：
         *  用 unsigned char 存储，是为了兼容两种格式
         *  渲染时根据 elementSize 选择 GL_UNSIGNED_SHORT/INT
         */
        void SetIndexBuffer(const void *data, size_t count, size_t elementSize);
        const unsigned char *GetIndexBuffer() const;
        size_t GetIndexCount() const;
        size_t GetIndexElementSize() const;

        void SetTextures(const std::vector<std::string> &textures);
        const std::vector<std::string> &GetTextures() const;

    private:
        std::vector<Vertex> m_vertices;           // 顶点缓冲区
        std::vector<unsigned char> m_indexBuffer; // 索引缓冲区（二进制存储）
        size_t m_indexCount = 0;                  // 索引数量
        size_t m_indexElementSize = 0;            // 每个索引的字节大小：2 或 4
        std::vector<std::string> m_textures;      // 纹理路径列表
    };
}