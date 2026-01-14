#pragma once

#include <map>
#include <string>
#include <vector>

#include "GLObject.h"

namespace miraipet::OpenGL
{

    /**
     * @brief OpenGL Shading Language 定义
     *
     */
    class GLSLDefine
    {
        using Map = std::map<std::string, std::string>;

    private:
        Map m_defines;

    public:
        bool operator==(const GLSLDefine &rhs) const { return m_defines == rhs.m_defines; }
        bool operator!=(const GLSLDefine &rhs) const { return m_defines != rhs.m_defines; }

        void Define(const std::string &def, const std::string &defValue = "");
        void Undefine(const std::string &def);
        void Clear();

        const Map &GetMap() const { return m_defines; }
    };

    /**
     * @brief OpenGL Shading Language 包含路径管理类
     */
    class GLSLInclude
    {
        using PathList = std::vector<std::string>;

    private:
        std::string m_workDir;
        PathList m_pathList;

    public:
        GLSLInclude(const std::string &workDir = "");

        void AddInclude(const std::string &include);
        void Clear();

        void SetWorkDir(const std::string &workDir) { m_workDir = workDir; }
        const std::string GetWorkDir() const { return m_workDir; }
        const PathList &GetPathList() const { return m_pathList; }
    };

    /**
     * @brief GLSL着色器语言类型
     */
    enum class GLSLShaderLang
    {
        Vertex,
        TessControl,
        TessEvaluation,
        Geometry,
        Fragment,
        Compute,
    };

    /**
     * @brief 预处理GLSL代码
     *
     * @param outCode       输出代码
     * @param lang          着色器语言类型
     * @param inCode        输入代码
     * @param define        定义
     * @param include       包含路径
     * @param outMessage    输出信息
     * @return true         预处理成功
     * @return false        预处理失败
     */
    bool PreprocessGLSL(
        std::string *outCode,
        GLSLShaderLang lang,
        const std::string &inCode,
        const GLSLDefine &define = GLSLDefine(),
        const GLSLInclude &include = GLSLInclude(),
        std::string *outMessage = nullptr);

    /**
     * @brief OpenGL Shading Language 工具类
     */
    class GLSLUtil
    {
    private:
        std::string m_shaderDir;
        GLSLDefine m_define;
        GLSLInclude m_include;

    public:
        GLSLUtil() = default;

        void SetShaderDir(const std::string &shaderDir);
        void SetGLSLDefine(const GLSLDefine &define);
        void SetGLSLInclude(const GLSLInclude &include);

        GLProgramObject CreateProgram(const char *shaderName);
        GLProgramObject CreateProgram(const char *vsCode, const char *fsCode);
    };

}