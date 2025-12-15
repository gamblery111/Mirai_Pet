#include "PetGLWidget.h"

namespace miraipet::ui
{

    PetGLWidget::PetGLWidget(QWidget *parent)
        : QOpenGLWidget(parent)
    {
    }

    void PetGLWidget::initializeGL()
    {
        
    }

    void PetGLWidget::paintGL()
    {
    }

} // namespace miraipet::ui
