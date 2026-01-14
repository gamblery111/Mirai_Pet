#include "MMDModelDrawContext.h"
#include "GLFacade.h"
#include "context/ViewerContext.h"
#include "macro.h"

namespace
{
    using OpGLSLUtil = miraipet::OpenGL::GLSLUtil;
}

namespace miraipet::MMD
{

    void MMDShader::Initialize()
    {
        // attribute
        m_inPos = GL::GetAttribLocation((GLuint)m_prog, "in_Pos");
        m_inNor = GL::GetAttribLocation((GLuint)m_prog, "in_Nor");
        m_inUV = GL::GetAttribLocation((GLuint)m_prog, "in_UV");

        // uniform
        m_uWV = GL::GetUniformLocation((GLuint)m_prog, "u_WV");
        m_uWVP = GL::GetUniformLocation((GLuint)m_prog, "u_WVP");

        m_uAmbinet = GL::GetUniformLocation((GLuint)m_prog, "u_Ambient");
        m_uDiffuse = GL::GetUniformLocation((GLuint)m_prog, "u_Diffuse");
        m_uSpecular = GL::GetUniformLocation((GLuint)m_prog, "u_Specular");
        m_uSpecularPower = GL::GetUniformLocation((GLuint)m_prog, "u_SpecularPower");
        m_uAlpha = GL::GetUniformLocation((GLuint)m_prog, "u_Alpha");

        m_uTexMode = GL::GetUniformLocation((GLuint)m_prog, "u_TexMode");
        m_uTex = GL::GetUniformLocation((GLuint)m_prog, "u_Tex");
        m_uTexMulFactor = GL::GetUniformLocation((GLuint)m_prog, "u_TexMulFactor");
        m_uTexAddFactor = GL::GetUniformLocation((GLuint)m_prog, "u_TexAddFactor");

        m_uSphereTexMode = GL::GetUniformLocation((GLuint)m_prog, "u_SphereTexMode");
        m_uSphereTex = GL::GetUniformLocation((GLuint)m_prog, "u_SphereTex");
        m_uSphereTexMulFactor = GL::GetUniformLocation((GLuint)m_prog, "u_SphereTexMulFactor");
        m_uSphereTexAddFactor = GL::GetUniformLocation((GLuint)m_prog, "u_SphereTexAddFactor");

        m_uToonTexMode = GL::GetUniformLocation((GLuint)m_prog, "u_ToonTexMode");
        m_uToonTex = GL::GetUniformLocation((GLuint)m_prog, "u_ToonTex");
        m_uToonTexMulFactor = GL::GetUniformLocation((GLuint)m_prog, "u_ToonTexMulFactor");
        m_uToonTexAddFactor = GL::GetUniformLocation((GLuint)m_prog, "u_ToonTexAddFactor");

        m_uLightColor = GL::GetUniformLocation((GLuint)m_prog, "u_LightColor");
        m_uLightDir = GL::GetUniformLocation((GLuint)m_prog, "u_LightDir");

        m_uLightVP = GL::GetUniformLocation((GLuint)m_prog, "u_LightWVP");
        m_uShadowMapSplitPositions = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMapSplitPositions");
        m_uShadowMap0 = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMap0");
        m_uShadowMap1 = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMap1");
        m_uShadowMap2 = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMap2");
        m_uShadowMap3 = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMap3");
        m_uShadowMapEnabled = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowMapEnabled");
    }

    void MMDEdgeShader::Initialize()
    {
        m_inPos = GL::GetAttribLocation((GLuint)m_prog, "in_Pos");
        m_inNor = GL::GetAttribLocation((GLuint)m_prog, "in_Nor");

        m_uWV = GL::GetUniformLocation((GLuint)m_prog, "u_WV");
        m_uWVP = GL::GetUniformLocation((GLuint)m_prog, "u_WVP");
        m_uScreenSize = GL::GetUniformLocation((GLuint)m_prog, "u_ScreenSize");
        m_uEdgeSize = GL::GetUniformLocation((GLuint)m_prog, "u_EdgeSize");
        m_uEdgeColor = GL::GetUniformLocation((GLuint)m_prog, "u_EdgeColor");
    }

    void MMDGroundShadowShader::Initialize()
    {
        m_inPos = GL::GetAttribLocation((GLuint)m_prog, "in_Pos");
        m_uWVP = GL::GetUniformLocation((GLuint)m_prog, "u_WVP");
        m_uShadowColor = GL::GetUniformLocation((GLuint)m_prog, "u_ShadowColor");
    }

    MMDModelDrawContext::MMDModelDrawContext(ViewerCtx *_ctxt)
        : m_pViewerContext(_ctxt)
    {
        MIRAI_ASSERT(_ctxt != nullptr);
    }

    int MMDModelDrawContext::GetShaderIndex(const OpGLSLDefine &_define)
    {
        if (nullptr == m_pViewerContext)
            return -1;

        auto it = std::find_if(m_shaders.begin(), m_shaders.end(), [&_define](const MMDShaderPtr &_shader)
                               { return _shader->m_define == _define; });

        // 如果存在直接返回索引
        if (it != m_shaders.end())
            return std::distance(m_shaders.begin(), it);

        // 如果不存在添加着色器
        MMDShaderPtr shader = std::make_unique<MMDShader>();
        shader->m_define = _define;
        OpGLSLUtil glslUtil;
        glslUtil.SetShaderDir(m_pViewerContext->GetShaderDir());
        glslUtil.SetGLSLDefine(_define);
        shader->m_prog = glslUtil.CreateProgram("mmd");
        if (!shader->m_prog)
        {
            return -1;
        }
        shader->Initialize();
        m_shaders.emplace_back(std::move(shader));
        return (int)(m_shaders.size() - 1);
    }

    MMDShader *MMDModelDrawContext::GetShader(int _shaderIndex) const
    {
        if (_shaderIndex < 0)
        {
            return nullptr;
        }

        return m_shaders[_shaderIndex].get();
    }

    int MMDModelDrawContext::GetEdgeShaderIndex(const OpGLSLDefine &_define)
    {
        if (nullptr == m_pViewerContext)
        {
            return -1;
        }
        auto it = std::find_if(m_edgeShaders.begin(), m_edgeShaders.end(), [&_define](const MMDEdgeShaderPtr &shader)
                               { return shader->m_define == _define; });

        // 如果存在直接返回索引
        if (it != m_edgeShaders.end())
            return std::distance(m_edgeShaders.begin(), it);

        // 如果不存在添加着色器
        MMDEdgeShaderPtr shader = std::make_unique<MMDEdgeShader>();
        shader->m_define = _define;
        OpGLSLUtil glslUtil;
        glslUtil.SetShaderDir(m_pViewerContext->GetShaderDir());
        glslUtil.SetGLSLDefine(_define);
        shader->m_prog = glslUtil.CreateProgram("mmd_edge");
        if (!shader->m_prog)
        {
            return -1;
        }
        shader->Initialize();
        m_edgeShaders.emplace_back(std::move(shader));
        return (int)(m_edgeShaders.size() - 1);
    }

    MMDEdgeShader *MMDModelDrawContext::GetEdgeShader(int _edgeShaderIndex) const
    {
        if (_edgeShaderIndex < 0)
        {
            return nullptr;
        }

        return m_edgeShaders[_edgeShaderIndex].get();
    }

    int MMDModelDrawContext::GetGroundShadowShaderIndex(const OpGLSLDefine &_define)
    {
        if (nullptr == m_pViewerContext)
        {
            return -1;
        }
        auto it = std::find_if(m_groundShadowShaders.begin(), m_groundShadowShaders.end(), [&_define](const MMDGroundShadowShaderPtr &shader)
                               { return shader->m_define == _define; });
        // 如果存在直接返回索引
        if (it != m_groundShadowShaders.end())
            return std::distance(m_groundShadowShaders.begin(), it);

        // 如果不存在添加着色器
        MMDGroundShadowShaderPtr shader = std::make_unique<MMDGroundShadowShader>();
        shader->m_define = _define;
        OpGLSLUtil glslUtil;
        glslUtil.SetShaderDir(m_pViewerContext->GetShaderDir());
        glslUtil.SetGLSLDefine(_define);
        shader->m_prog = glslUtil.CreateProgram("mmd_ground_shadow");
        if (!shader->m_prog)
        {
            return -1;
        }
        shader->Initialize();
        m_groundShadowShaders.emplace_back(std::move(shader));
        return (int)(m_groundShadowShaders.size() - 1);
    }

    MMDGroundShadowShader *MMDModelDrawContext::GetGroundShadowShader(int _groundShadowShaderIndex) const
    {
        if (_groundShadowShaderIndex < 0)
        {
            return nullptr;
        }
        return m_groundShadowShaders[_groundShadowShaderIndex].get();
    }

    MMDModelDrawContext::ViewerCtx *MMDModelDrawContext::GetViewerContext() const
    {
        return m_pViewerContext;
    }

} // namespace miraipet::MMD