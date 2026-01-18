#pragma once

#include <glm/vec3.hpp>

namespace miraipet::view
{
    class Light
    {
    private:
        glm::vec3 m_lightColor;
        glm::vec3 m_lightDir;

    public:
        Light();

        void SetLightColor(const glm::vec3 &color);
        const glm::vec3 &GetLightColor() const;

        void SetLightDirection(const glm::vec3 &dir);
        const glm::vec3 &GetLightDirection() const;
    };
}