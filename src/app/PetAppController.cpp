#include "PetAppController.h"
#include "context/RenderContext.h"
#include "model/PetModelData.h"
#include "render/PetRenderer.h"
#include "render/PetShaderManager.h"
#include "PetGLWidget.h"
#include "MMDLoader.h"

#include <QOpenGLWidget>

namespace miraipet::ui
{

    PetAppController::PetAppController(PetGLWidget *widget)
    {
        InitContext(widget);
        // Renderer
        m_renderer = std::make_unique<PetRender>(m_context.get());

        // 加载模型
        LoadModel();
    }

    PetAppController::~PetAppController()
    {
    }

    void PetAppController::Frame()
    {
        m_renderer->Render();
    }

    void PetAppController::LoadModel()
    {
        const std::string modelPath = "E:/MMd/haila/海拉3.0.pmx";
        const std::string dataDir = "E:/MMd/haila";
        MMD::MMDLoader loader;
        auto modelData = loader.load(modelPath, dataDir);

        // 将 modelData 传递给渲染器
        if (modelData) {
           m_context->currentModelData = modelData;
           m_renderer->UpdateModelData();
        }
    }

    void PetAppController::InitContext(PetGLWidget *widget)
    {
        m_context = std::make_unique<RenderCtx>();

        m_context->gl = static_cast<QOpenGLFunctions_3_3_Core *>(widget);

        assert(m_context->gl != nullptr);

        // ShaderManager
        m_context->shaderManager = new render::PetShaderManager(m_context->gl);
    }
}