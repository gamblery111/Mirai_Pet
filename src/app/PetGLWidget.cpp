#include "PetGLWidget.h"
#include "PetAppController.h"

namespace miraipet::ui
{

    PetGLWidget::PetGLWidget(QWidget *parent)
        : QOpenGLWidget(parent)
    {
    }

    PetGLWidget::~PetGLWidget()
    {
    }

    void PetGLWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        m_controller = std::make_unique<PetAppController>(this);
    }

    void PetGLWidget::resizeGL(int w, int h)
    {
        if (m_controller) {
            m_controller->Resize(w, h);
        }
    }

    void PetGLWidget::paintGL()
    {
        m_controller->Frame();
    }

} // namespace miraipet::ui
