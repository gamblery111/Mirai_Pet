#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

#include "GLObject.h"

namespace miraipet::context
{
    class ViewerContext;
} // namespace miraiPet::context

namespace miraipet::view
{
    class Camera;
    class Light;

    struct ShadowMapShader
    {
        OpenGL::GLProgramObject m_prog; ///< 程序对象
        GLint m_inPos;                  ///< 位置属性
        GLint m_uWVP;                   ///< 变换矩阵

        void Initialize();
    };

    /**
     * @brief 阴影映射
     */
    class ShadowMap
    {
        using ViewCtxPtr = context::ViewerContext *;
        
        struct ClipSpace
        {
            float fNearClip;                          ///< 近裁剪平面
            float fFarClip;                           ///< 远裁剪平面
            glm::mat4 Projection;                     ///< 投影矩阵
            OpenGL::GLTextureObject Shadomap;         ///< 阴影贴图
            OpenGL::GLFramebufferObject ShadowmapFBO; ///< 阴影映射帧缓冲区对象
        };

    private:
        glm::mat4 m_view;                    ///< 视图矩阵
        std::vector<ClipSpace> m_clipSpaces; ///< 裁剪空间
        std::vector<float> m_splitPositions; ///< 分割位置
        size_t m_nSplitCount;                ///< 分割数量
        ShadowMapShader m_shader;            ///< 阴影映射着色器
        int m_nWidth;                        ///< 阴影贴图宽度
        int m_nHeight;                       ///< 阴影贴图高度
        float m_fNearClip;                   ///< 近裁剪平面
        float m_fFarClip;                    ///< 远裁剪平面
        float m_fBias;                       ///< 偏移量

    public:
        ShadowMap();
        ShadowMap(const ShadowMap &) = delete;
        ShadowMap &operator=(const ShadowMap &) = delete;

        bool InitializeShader(ViewCtxPtr _ctxt);
    };

} // namespace miraipet::view