#include "PetAppController.h"
#include "context/RenderContext.h"
#include "model/PetModelData.h"
#include "render/PetRenderer.h"
#include "render/PetShaderManager.h"
#include "PetGLWidget.h"
#include "MMDLoader.h"

#include <QOpenGLWidget>
#include <QWidget>
#include <algorithm>
#include <QScreen>

namespace miraipet::ui
{

    PetAppController::PetAppController(PetGLWidget *widget)
        : m_widget(widget)
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

    void PetAppController::Resize(int w, int h)
    {
        m_context->aspectRatio = static_cast<float>(w) / static_cast<float>(h);
        m_renderer->ResizeGL(w, h);
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
           // 计算模型边界框
           modelData->CalculateBoundingBox();
           m_renderer->UpdateModelData();

           // 根据模型边界框自动调整窗口大小
           AdjustWindowToModel();
        }
    }

    void PetAppController::InitContext(PetGLWidget *widget)
    {
        m_context = std::make_unique<RenderCtx>();

        // ShaderManager
        m_context->shaderManager = new render::PetShaderManager();

        // 初始化宽高比
        if (widget) {
            int w = widget->width();
            int h = widget->height();
            if (h > 0) {
                m_context->aspectRatio = static_cast<float>(w) / static_cast<float>(h);
            }
        }
    }

    void PetAppController::AdjustWindowToModel()
    {
        if (!m_context->currentModelData || !m_widget)
            return;

        const auto &bbox = m_context->currentModelData->GetBoundingBox();

        // 计算模型的尺寸
        float modelWidthMM = bbox[3] - bbox[0]; 
        float modelHeightMM = bbox[4] - bbox[1];

        // 使用DPI感知的像素密度，让单位更精确
        QScreen *screen = m_widget->screen();
        float dpi = screen->physicalDotsPerInch();
        float pixelsPerMM = dpi / 25.4f;
        
        // 使用物理单位直接转换（DPI感知）
        int windowWidth = static_cast<int>(modelWidthMM * pixelsPerMM);
        int windowHeight = static_cast<int>(modelHeightMM * pixelsPerMM);

        // 调整主窗口大小
        if (auto *parentWidget = m_widget->parentWidget()) {
            parentWidget->resize(windowWidth, windowHeight);
            m_widget->resize(windowWidth, windowHeight);
        }
    }
}