#include <iostream>

#include "GLSLUtil.h"
#include "PathUtil.h"
#include "FileUtil.h"
#include "UnicodeUtil.h"
#include "GLShaderUtil.h"

extern "C"
{
#include <fpp.h>
}

namespace miraipet::OpenGL
{
    namespace
    {
        struct FppData
        {
            std::string::const_iterator m_inputIt;
            std::string::const_iterator m_inputEnd;

            std::stringstream m_output;
            std::stringstream m_error;
        };

        char *FppInput(char *buffer, int size, void *userData)
        {
            FppData *fppData = reinterpret_cast<FppData *>(userData);

            if (fppData->m_inputIt == fppData->m_inputEnd)
            {
                return nullptr;
            }

            int i = 0;
            while (fppData->m_inputIt != fppData->m_inputEnd)
            {
                buffer[i] = (*fppData->m_inputIt);
                fppData->m_inputIt++;
                i++;
                if (i == size)
                {
                    buffer[i] = '\0';
                    break;
                }
            }
            if (i != size)
            {
                buffer[i] = '\0';
            }
            return buffer;
        }

        void FppOutput(int ch, void *userData)
        {
            FppData *fppData = reinterpret_cast<FppData *>(userData);

            fppData->m_output << (char)ch;
        }

        void FppError(void *userData, char *format, va_list vargs)
        {
            FppData *fppData = reinterpret_cast<FppData *>(userData);

            char buffer[1024];
            vsnprintf(buffer, sizeof(buffer), format, vargs);
            fppData->m_error << "fpp error : " << buffer << "\n";
        }

        FILE *FppFileopen(const char *filename, void *userdata)
        {
            FILE *fp = NULL;
            std::wstring wFilepath;
            if (!BaseUtils::TryToWString(filename, wFilepath))
            {
                return nullptr;
            }
            auto err = _wfopen_s(&fp, wFilepath.c_str(), L"r");
            if (err != 0)
            {
                return nullptr;
            }
            return fp;
        }

        std::vector<char> MakeStringBuffer(const std::string &str)
        {
            std::vector<char> buffer;
            buffer.reserve(str.size() + 1);
            buffer.assign(str.begin(), str.end());
            buffer.push_back('\0');
            return buffer;
        }
    }

    void GLSLDefine::Define(const std::string &def, const std::string &defValue)
    {
        m_defines[def] = defValue;
    }

    void GLSLDefine::Undefine(const std::string &def)
    {
        m_defines.erase(def);
    }

    void GLSLDefine::Clear()
    {
        m_defines.clear();
    }

    GLSLInclude::GLSLInclude(const std::string &workDir)
        : m_workDir(workDir)
    {
    }

    void GLSLInclude::AddInclude(const std::string &include)
    {
        m_pathList.push_back(include);
    }

    void GLSLInclude::Clear()
    {
        m_pathList.clear();
    }

    bool PreprocessGLSL(std::string *outCode, GLSLShaderLang lang, const std::string &inCode, const GLSLDefine &define, const GLSLInclude &include, std::string *outMessage)
    {
        FppData fppData;
        fppData.m_inputIt = inCode.begin();
        fppData.m_inputEnd = inCode.end();

        std::vector<fppTag> tags;
        fppTag tag;
        tag.tag = FPPTAG_USERDATA;
        tag.data = &fppData;
        tags.push_back(tag);

        tag.tag = FPPTAG_INPUT;
        tag.data = (void *)FppInput;
        tags.push_back(tag);

        tag.tag = FPPTAG_OUTPUT;
        tag.data = (void *)FppOutput;
        tags.push_back(tag);

        tag.tag = FPPTAG_ERROR;
        tag.data = (void *)FppError;
        tags.push_back(tag);

        tag.tag = FPPTAG_FILE_OPEN_EXT;
        tag.data = (void *)FppFileopen;
        tags.push_back(tag);

        tag.tag = FPPTAG_IGNOREVERSION;
        tag.data = (void *)0;
        tags.push_back(tag);

        tag.tag = FPPTAG_LINE;
        tag.data = (void *)0;
        tags.push_back(tag);

        auto includeList = include.GetPathList();
        std::vector<std::vector<char>> includeDirs;
        const auto &workDir = include.GetWorkDir();
        if (!workDir.empty())
        {
            includeDirs.emplace_back(MakeStringBuffer(workDir));
        }
        for (const auto &includeDir : includeList)
        {
            includeDirs.emplace_back(MakeStringBuffer(includeDir));
        }
        for (auto &includeDir : includeDirs)
        {
            tag.tag = FPPTAG_INCLUDE_DIR;
            tag.data = (void *)includeDir.data();
            tags.push_back(tag);
        }

        const char *filename = "GLSL";
        tag.tag = FPPTAG_INPUT_NAME;
        tag.data = (void *)filename;
        tags.push_back(tag);

        const auto &defineMap = define.GetMap();
        std::vector<std::vector<char>> defineStrs;
        for (const auto &definePair : defineMap)
        {
            const std::string &key = definePair.first;
            const std::string &val = definePair.second;
            if (val.empty())
            {
                defineStrs.emplace_back(MakeStringBuffer(key));
            }
            else
            {
                defineStrs.emplace_back(MakeStringBuffer(key + "=" + val));
            }
        }
        for (auto &defineStr : defineStrs)
        {
            tag.tag = FPPTAG_DEFINE;
            tag.data = (void *)defineStr.data();
            tags.push_back(tag);
        }

        tag.tag = FPPTAG_END;
        tag.data = nullptr;
        tags.push_back(tag);

        auto ret = fppPreProcess((fppTag *)tags.data());
        auto errorMessage = fppData.m_error.str();

        if (outMessage != nullptr)
        {
            *outMessage = errorMessage;
        }
        if (ret != 0 || !errorMessage.empty())
        {
            return false;
        }
        *outCode = fppData.m_output.str();

        return true;
    }

    void GLSLUtil::SetShaderDir(const std::string &shaderDir)
    {
        m_shaderDir = shaderDir;
    }

    void GLSLUtil::SetGLSLDefine(const GLSLDefine &define)
    {
        m_define = define;
    }

    void GLSLUtil::SetGLSLInclude(const GLSLInclude &include)
    {
        m_include = include;
    }

    GLProgramObject GLSLUtil::CreateProgram(const char *shaderName)
    {
        std::string shaderFilePath = BaseUtils::PathUtil::Combine(m_shaderDir, shaderName);
        std::string vsFilePath = shaderFilePath + ".vert";
        std::string fsFilePath = shaderFilePath + ".frag";

        std::string vsCode;
        {
            BaseUtils::TextFileReader glslFile(vsFilePath);
            if (!glslFile.IsOpen())
            {
                return GLProgramObject();
            }
            vsCode = glslFile.ReadAll();
        }

        std::string fsCode;
        {
            BaseUtils::TextFileReader glslFile(fsFilePath);
            if (!glslFile.IsOpen())
            {
                return GLProgramObject();
            }
            fsCode = glslFile.ReadAll();
        }

        return CreateProgram(vsCode.c_str(), fsCode.c_str());
    }

    GLProgramObject GLSLUtil::CreateProgram(const char *vsCode, const char *fsCode)
    {
        GLSLInclude include = m_include;
        if (!m_shaderDir.empty())
        {
            include.AddInclude(m_shaderDir);
        }

        std::string ppMessage;
        std::string ppVsCode;
        bool ret = PreprocessGLSL(
            &ppVsCode,
            GLSLShaderLang::Vertex,
            vsCode,
            m_define,
            include,
            &ppMessage);
        if (!ret)
        {
            std::cout << "preprocess fail.\n";
            std::cout << ppMessage;
            return GLProgramObject();
        }

        std::string ppFsCode;
        ret = PreprocessGLSL(
            &ppFsCode,
            GLSLShaderLang::Fragment,
            fsCode,
            m_define,
            include,
            &ppMessage);
        if (!ret)
        {
            std::cout << "preprocess fail.\n";
            std::cout << ppMessage;
            return GLProgramObject();
        }

        return CreateShaderProgram(ppVsCode.c_str(), ppFsCode.c_str());
    }

}