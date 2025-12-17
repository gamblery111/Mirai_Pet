#pragma once

#include <QMainWindow>

namespace miraipet::ui
{

    class PetGLWidget;
    class PetMainWindow : public QMainWindow
    {
        // Q_OBJECT
    private:
        PetGLWidget *m_glWidget;

    public:
        PetMainWindow();
        ~PetMainWindow();

    };

}