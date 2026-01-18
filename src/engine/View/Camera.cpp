#include "Camera.h"

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace miraipet::view
{
    Camera::Camera()
        : m_target(0.0f), m_eye(0.0f), m_up(0.0f), m_fRadius(1.0f), m_fFovYRad(glm::radians(30.0f)), m_fNearClip(0.01f), m_fFarClip(100.0f), m_fWidth(0.0f), m_fHeight(0.0f), m_viewMatrix(1.0f), m_projectionMatrix(1.0f)
    {
    }

    void Camera::Initialize(const glm::vec3 &center, float radius)
    {
        m_target = center;
        m_eye = center + glm::vec3(0.5f, 0.5f, 1) * radius * 5.0f; // 相机位置在目标点的正前方
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);                        // 相机上方向为Y轴
        m_fRadius = radius;

        m_fNearClip = radius * 0.01f;
        m_fFarClip = radius * 100.0f;

        UpdateMatrix();
    }

    void Camera::UpdateMatrix()
    {
        m_viewMatrix = glm::lookAtRH(m_eye, m_target, m_up);
        if (m_fWidth <= 0.0f || m_fHeight <= 0.0f)
        {
            return;
        }
        // 调用透视投影矩阵函数
        m_projectionMatrix = glm::perspectiveFovRH(m_fFovYRad, m_fWidth, m_fHeight, m_fNearClip, m_fFarClip);
    }

} // namespace miraipet::view
