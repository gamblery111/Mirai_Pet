#include "PetAppController.h"
#include "context/RenderContext.h"
#include "render/PetRenderer.h"
#include "render/PetShaderManager.h"
#include "PetGLWidget.h"

#include <QOpenGLWidget>

namespace miraipet::ui
{

    PetAppController::PetAppController(PetGLWidget *widget)
    {
        m_context = std::make_unique<RenderCtx>();

        m_context->gl = static_cast<QOpenGLFunctions_3_3_Core *>(widget);
        
        assert(m_context->gl != nullptr);
        
        // ShaderManager
        m_context->shaderManager = new render::PetShaderManager(m_context->gl);

        // Renderer
        m_renderer = std::make_unique<PetRender>(m_context.get());
    }

    PetAppController::~PetAppController()
    {
    }

    void PetAppController::Frame()
    {
        m_renderer->Render();
    }
}