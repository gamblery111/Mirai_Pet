#pragma once
#include <memory>
#include <string>

namespace miraipet::render { class PetRenderer; }
namespace miraipet::context { struct RenderContext; }

namespace miraipet::ui
{
    
    class PetGLWidget;
    class PetAppController
    {
        using RenderCtx = context::RenderContext;
        using PetRender = render::PetRenderer;
        
    private:
        std::unique_ptr<RenderCtx>  m_context;
        std::unique_ptr<PetRender>  m_renderer;
        PetGLWidget* m_widget;
        
    public:
        PetAppController(PetGLWidget* widget);
        ~PetAppController();

        void Frame();
        void LoadModel();
        void Resize(int w, int h);

    private:
        void InitContext(PetGLWidget *widget);
        void AdjustWindowToModel();
    };
}
