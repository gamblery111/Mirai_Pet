#pragma once
#include <memory>
#include <string>

namespace miraipet::context
{
    class ViewerContext;
}

namespace miraipet::ui
{

    class PetGLWidget;
    class PetAppController
    {
    private:
        using ViewCtx = context::ViewerContext;

    private:
        std::unique_ptr<ViewCtx> m_upViewerCtx;

    public:
        PetAppController();
        ~PetAppController();
    };
}
