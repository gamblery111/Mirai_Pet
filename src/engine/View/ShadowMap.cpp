#include "ShadowMap.h"

#include "GLFacade.h"
#include "GLSLUtil.h"
#include "context/ViewerContext.h"

namespace miraipet::view
{

    void miraipet::view::ShadowMapShader::Initialize()
    {
        m_inPos = GL::GetAttribLocation((GLuint)m_prog, "in_Pos");
        m_uWVP = GL::GetUniformLocation((GLuint)m_prog, "u_WVP");
    }

    ShadowMap::ShadowMap()
        : m_nSplitCount(4), m_nWidth(1024), m_nHeight(1024), m_fNearClip(0.01f), m_fFarClip(1000.0f), m_fBias(0.01f)
    {
    }

    bool ShadowMap::InitializeShader(ViewCtxPtr _ctxt)
    {
        OpenGL::GLSLUtil glslUtil;
        glslUtil.SetShaderDir(_ctxt->GetShaderDir());
        m_shader.m_prog = glslUtil.CreateProgram("shadow_shader");
        if (!m_shader.m_prog)
        {
            return false;
        }
        m_shader.Initialize();
        return true;
    }

} // namespace miraipet::view