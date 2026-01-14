#pragma once

#include "GLSLUtil.h"

namespace miraipet::context
{
    class ViewerContext;
}

namespace miraipet::MMD
{
    /**
     * @brief MMDShader结构体，用于管理着色器程序
     */
    struct MMDShader
    {
        OpenGL::GLSLDefine m_define;    ///< GLSL定义
        OpenGL::GLProgramObject m_prog; ///< GL程序对象

        GLint m_inPos; ///< 顶点位置属性
        GLint m_inNor; ///< 顶点法线属性
        GLint m_inUV;  ///< UV属性

        GLint m_uWV;                      ///< 世界-视图矩阵
        GLint m_uWVP;                     ///< 世界-视图-投影矩阵
        GLint m_uAmbinet;                 ///< 环境光颜色
        GLint m_uDiffuse;                 ///< 漫反射颜色
        GLint m_uSpecular;                ///< 高光颜色
        GLint m_uSpecularPower;           ///< 高光强度
        GLint m_uAlpha;                   ///< 透明度
        GLint m_uTexMode;                 ///< 纹理模式
        GLint m_uTex;                     ///< 纹理
        GLint m_uTexMulFactor;            ///< 纹理乘法因子
        GLint m_uTexAddFactor;            ///< 纹理加法因子
        GLint m_uSphereTexMode;           ///< 球面纹理模式
        GLint m_uSphereTex;               ///< 球面纹理
        GLint m_uSphereTexMulFactor;      ///< 球面纹理乘法因子
        GLint m_uSphereTexAddFactor;      ///< 球面纹理加法因子
        GLint m_uToonTexMode;             ///< 辉光纹理模式
        GLint m_uToonTex;                 ///< 辉光纹理
        GLint m_uToonTexMulFactor;        ///< 辉光纹理乘法因子
        GLint m_uToonTexAddFactor;        ///< 辉光纹理加法因子
        GLint m_uLightColor;              ///< 光源颜色
        GLint m_uLightDir;                ///< 光源方向
        GLint m_uLightVP;                 ///< 光源视图矩阵
        GLint m_uShadowMapSplitPositions; ///< 阴影贴影切分位置
        GLint m_uShadowMap0;              ///< 阴影贴影0
        GLint m_uShadowMap1;              ///< 阳影贴影1
        GLint m_uShadowMap2;              ///< 阳影贴影2
        GLint m_uShadowMap3;              ///< 阳影贴影3
        GLint m_uShadowMapEnabled;        ///< 是否启用阴影贴影

        /**
         * @brief 初始化着色器程序
         */
        void Initialize();
    };

    /**
     * @brief MMDEdgeShader结构体，用于管理MMD边缘着色器程序
     */
    struct MMDEdgeShader
    {
        OpenGL::GLSLDefine m_define;    ///< GLSL定义
        OpenGL::GLProgramObject m_prog; ///< GL程序对象

        GLint m_inPos; ///< 顶点位置属性
        GLint m_inNor; ///< 顶点法线属性

        GLint m_uWV;         ///< 世界-视图矩阵
        GLint m_uWVP;        ///< 世界-视图-投影矩阵
        GLint m_uScreenSize; ///< 屏幕尺寸
        GLint m_uEdgeSize;   ///< 边缘尺寸

        GLint m_uEdgeColor; ///< 边缘颜色

        void Initialize();
    };

    /**
     * @brief MMDGroundShadowShader结构体，用于管理MMD地面阴影着色器程序
     */
    struct MMDGroundShadowShader
    {
        OpenGL::GLSLDefine m_define;    ///< GLSL定义
        OpenGL::GLProgramObject m_prog; ///< GL程序对象

        GLint m_inPos; ///< 顶点位置属性

        GLint m_uWVP;         ///< 世界-视图-投影矩阵
        GLint m_uShadowColor; ///< 阴影颜色

        void Initialize();
    };

    /**
     * @brief MMD模型绘制上下文类
     */
    class MMDModelDrawContext
    {
    private:
        using MMDShaderPtr = std::unique_ptr<MMDShader>;
        using MMDEdgeShaderPtr = std::unique_ptr<MMDEdgeShader>;
        using MMDGroundShadowShaderPtr = std::unique_ptr<MMDGroundShadowShader>;
        using OpGLSLDefine = OpenGL::GLSLDefine;
        using ViewerCtx = context::ViewerContext;
    private:
        ViewerCtx *m_pViewerContext;                             ///< 视图上下文指针, non-owning
        std::vector<MMDShaderPtr> m_shaders;                         ///< MMD着色器程序列表
        std::vector<MMDEdgeShaderPtr> m_edgeShaders;                 ///< MMD边缘着色器程序列表
        std::vector<MMDGroundShadowShaderPtr> m_groundShadowShaders; ///< MMD地面阴影着色器程序列表

    public:
        MMDModelDrawContext(ViewerCtx *_ctxt);

        MMDModelDrawContext(const MMDModelDrawContext &) = delete;
        MMDModelDrawContext &operator=(const MMDModelDrawContext &) = delete;

        /**
         * @brief 获取着色器索引
         * @param _define   GLSL定义
         * @return int      着色器索引,-1为无效值
         */
        int GetShaderIndex(const OpGLSLDefine &_define);
        MMDShader *GetShader(int _shaderIndex) const;

        int GetEdgeShaderIndex(const OpGLSLDefine &_define);
        MMDEdgeShader *GetEdgeShader(int _edgeShaderIndex) const;

        int GetGroundShadowShaderIndex(const OpGLSLDefine &_define);
        MMDGroundShadowShader *GetGroundShadowShader(int _groundShadowShaderIndex) const;

        ViewerCtx *GetViewerContext() const;
    };

} // namespace miraipet::MMD