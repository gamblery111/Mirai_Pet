#pragma once

#include <string>

#include "GLObject.h"

namespace miraipet::context
{

    class ViewerContext
    {
        using GLTextureRef = OpenGL::GLTextureRef;

    private:
        std::string m_workDir;     ///< 工作目录
        std::string m_resourceDir; ///< 资源目录
        std::string m_shaderDir;   ///< 着色器目录

        GLTextureRef m_dummyColorTexture;       ///< 空颜色纹理
        GLTextureRef m_dummyShadowDepthTexture; ///< 空阴影深度纹理
        GLTextureRef m_captureTex;              ///< 捕获纹理

        int m_frameBufferWidth;  ///< 帧缓冲区宽度
        int m_frameBufferHeight; ///< 帧缓冲区高度

    public:
        ViewerContext();

        bool Initialize();
        void Uninitialize();
        
        const std::string &GetWorkDir() const { return m_workDir; }
        const std::string &GetResourceDir() const { return m_resourceDir; }
        const std::string &GetShaderDir() const { return m_shaderDir; }
    };

}