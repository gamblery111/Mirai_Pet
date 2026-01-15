#include "ViewerContext.h"
#include "PathUtil.h"

namespace miraipet::context
{
    ViewerContext::ViewerContext()
    {
        m_workDir = BaseUtils::PathUtil::GetExecutableDir();
        m_resourceDir = BaseUtils::PathUtil::Combine(m_workDir, "resources");
        m_shaderDir = BaseUtils::PathUtil::Combine(m_resourceDir, "shader");
    }
}