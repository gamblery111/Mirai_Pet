#include "ViewerContext.h"
#include "PathUtil.h"

namespace miraipet::context
{
    ViewerContext::ViewerContext()
        : m_frameBufferWidth(0), m_frameBufferHeight(0)
    {
        m_workDir = BaseUtils::PathUtil::GetExecutableDir();
        m_resourceDir = BaseUtils::PathUtil::Combine(m_workDir, "resources");
        m_shaderDir = BaseUtils::PathUtil::Combine(m_resourceDir, "shader");
    }

    bool ViewerContext::Initialize()
    {
        // 创建一个空的颜色纹理，用于渲染到帧缓冲区
        OpenGL::GLTextureObject dummyColorTex;
        if (!dummyColorTex.Create())
        {
            return false;
        }
        // 绑定纹理并设置为空数据
        GL::BindTexture(GL_TEXTURE_2D, (GLuint)dummyColorTex);
        GL::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        GL::BindTexture(GL_TEXTURE_2D, 0);
        m_dummyColorTexture = std::move(dummyColorTex);

        // 创建一个空的深度纹理，用于阴影映射
        OpenGL::GLTextureObject dummyShadowDepthTex;
        if (!dummyShadowDepthTex.Create())
        {
            return false;
        }
        GL::BindTexture(GL_TEXTURE_2D, (GLuint)dummyShadowDepthTex);
        GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        GL::BindTexture(GL_TEXTURE_2D, 0);
        m_dummyShadowDepthTexture = std::move(dummyShadowDepthTex);

        // 创建一个空的颜色纹理，用于捕获渲染结果
        OpenGL::GLTextureObject captureTex;
        if (!captureTex.Create())
        {
            return false;
        }
        // 绑定纹理并设置为空数据
        glBindTexture(GL_TEXTURE_2D, (GLuint)captureTex);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            m_frameBufferWidth,
            m_frameBufferHeight,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_captureTex = std::move(captureTex);

        return true;
    }

    void ViewerContext::Uninitialize()
    {
        m_dummyColorTexture.Release();
        m_dummyShadowDepthTexture.Release();
        m_captureTex.Release();
    }
    
}