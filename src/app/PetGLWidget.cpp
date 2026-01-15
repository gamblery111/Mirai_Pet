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
        m_controller = std::make_unique<PetAppController>();
    }

    void PetGLWidget::resizeGL(int w, int h)
    {
    }

    void PetGLWidget::paintGL()
    {
    }

} // namespace miraipet::ui
