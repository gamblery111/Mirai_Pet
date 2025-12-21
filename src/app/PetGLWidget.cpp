#include "PetGLWidget.h"
#include "PetAppController.h"

namespace miraipet::ui
{

    PetGLWidget::PetGLWidget(QWidget *parent)
        : QOpenGLWidget(parent)
    {
        // 设置 OpenGL 3.3 Core Profile
        QSurfaceFormat fmt;
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
        setFormat(fmt);
    }

    PetGLWidget::~PetGLWidget()
    {
    }

    void PetGLWidget::initializeGL()
    {
        initializeOpenGLFunctions(); // 初始化 3.3 核心函数
        m_controller = std::make_unique<PetAppController>(this);
    }

    void PetGLWidget::resizeGL(int w, int h)
    {
    }

    void PetGLWidget::paintGL()
    {
        m_controller->Frame();
    }

} // namespace miraipet::ui
