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

        // 计算模型的尺寸（MMD模型通常使用毫米为单位）
        float modelWidthMM = bbox[3] - bbox[0];  // maxX - minX (毫米)
        float modelHeightMM = bbox[4] - bbox[1]; // maxY - minY (毫米)

        // 将毫米转换为像素，减小转换因子和边距让窗口更紧凑
        const float mmToPixel = 1.5f;  // 减小转换因子
        int windowWidth = static_cast<int>(modelWidthMM * mmToPixel + 50);   // 减小边距
        int windowHeight = static_cast<int>(modelHeightMM * mmToPixel + 50); // 减小边距

        // 设置最小和最大窗口尺寸
        windowWidth = std::max(windowWidth, 400);
        windowHeight = std::max(windowHeight, 300);
        windowWidth = std::min(windowWidth, 1200);   // 最大宽度限制
        windowHeight = std::min(windowHeight, 900);  // 最大高度限制

        // 调整主窗口大小
        if (auto *parentWidget = m_widget->parentWidget()) {
            parentWidget->resize(windowWidth, windowHeight);
            m_widget->resize(windowWidth, windowHeight);
        }
    }
}
