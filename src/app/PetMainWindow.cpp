#include "PetMainWindow.h"
#include "PetGLWidget.h"

namespace miraipet::ui
{
    PetMainWindow::PetMainWindow()
    {
        m_glWidget = new PetGLWidget(this);
        setCentralWidget(m_glWidget);
    }
    PetMainWindow::~PetMainWindow()
    {
    }
    
}
