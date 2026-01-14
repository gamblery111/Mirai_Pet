#pragma once

#include <string>

namespace miraipet::context
{

    class ViewerContext
    {
    private:
        std::string m_workDir;
        std::string m_resourceDir;
        std::string m_shaderDir;

    public:
        ViewerContext();

        const std::string &GetWorkDir() const { return m_workDir; }
        const std::string &GetResourceDir() const { return m_resourceDir; }
        const std::string &GetShaderDir() const { return m_shaderDir; }
    };

}