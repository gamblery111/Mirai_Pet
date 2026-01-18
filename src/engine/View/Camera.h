#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace miraipet::view
{

    /**
     * @brief  相机
     */
    class Camera
    {
    private:
        glm::vec3 m_target; ///< 目标点
        glm::vec3 m_eye;    ///< 相机位置
        glm::vec3 m_up;     ///< 相机上方向
        float m_fRadius;    ///< 相机半径

        float m_fFovYRad;  ///< 相机垂直视野角度（弧度）
        float m_fNearClip; ///< 相机近裁剪面距离
        float m_fFarClip;  ///< 相机远裁剪面距离
        float m_fWidth;    ///< 相机宽度
        float m_fHeight;   ///< 相机高度

        glm::mat4 m_viewMatrix;       ///< 相机视图矩阵
        glm::mat4 m_projectionMatrix; ///< 相机投影矩阵

    public:
        Camera();

        void Initialize(const glm::vec3 &center, float radius);

        /**
         * @brief 更新相机视图矩阵和投影矩阵
         */
        void UpdateMatrix();
    };

} // namespace miraipet::view