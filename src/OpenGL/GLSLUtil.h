#pragma once

#include <map>
#include <string>
#include <vector>

#include "GLObject.h"

namespace miraipet::OpenGL
{
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

	enum class GLSLShaderLang
	{
		Vertex,
		TessControl,
		TessEvaluation,
		Geometry,
		Fragment,
		Compute,
	};

	bool PreprocessGLSL(
		std::string*		outCode,
		GLSLShaderLang		lang,
		const std::string&	inCode,
		const GLSLDefine&	define = GLSLDefine(),
		const GLSLInclude&	include = GLSLInclude(),
		std::string*		outMessage = nullptr
	);

    /**
     * @brief GLShader Language Utilities
     */
    class GLSLUtil
    {
    private:
        std::string m_shaderDir;
        GLSLDefine m_define;
        GLSLInclude m_include;
        
	public:
		GLSLUtil() = default;

		void SetShaderDir(const std::string& shaderDir);
		void SetGLSLDefine(const GLSLDefine& define);
		void SetGLSLInclude(const GLSLInclude& include);

		GLProgramObject CreateProgram(const char* shaderName);
		GLProgramObject CreateProgram(const char* vsCode, const char* fsCode);
    };

}