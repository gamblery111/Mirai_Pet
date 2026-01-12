#pragma once

#include "GLSLUtil.h"

namespace miraipet::MMD
{
    struct MMDShader
    {
        OpenGL::GLSLDefine m_define;
        OpenGL::GLProgramObject m_prog;

        // attribute
        GLint m_inPos;
        GLint m_inNor;
        GLint m_inUV;

        // uniform
        GLint m_uWV;
        GLint m_uWVP;

        GLint m_uAmbinet;
        GLint m_uDiffuse;
        GLint m_uSpecular;
        GLint m_uSpecularPower;
        GLint m_uAlpha;

        GLint m_uTexMode;
        GLint m_uTex;
        GLint m_uTexMulFactor;
        GLint m_uTexAddFactor;

        GLint m_uSphereTexMode;
        GLint m_uSphereTex;
        GLint m_uSphereTexMulFactor;
        GLint m_uSphereTexAddFactor;

        GLint m_uToonTexMode;
        GLint m_uToonTex;
        GLint m_uToonTexMulFactor;
        GLint m_uToonTexAddFactor;

        GLint m_uLightColor;
        GLint m_uLightDir;

        GLint m_uLightVP;
        GLint m_uShadowMapSplitPositions;
        GLint m_uShadowMap0;
        GLint m_uShadowMap1;
        GLint m_uShadowMap2;
        GLint m_uShadowMap3;
        GLint m_uShadowMapEnabled;

        void Initialize();
    };

    class MMDModelDrawContext
    {
    public:
        MMDModelDrawContext();
        ~MMDModelDrawContext();

    private:
    };

} // namespace miraipet::MMD