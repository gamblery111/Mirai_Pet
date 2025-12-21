#pragma once

#include <QMainWindow>

namespace miraipet::ui
{

    class PetGLWidget;
    class PetMainWindow : public QMainWindow
    {
    private:
        PetGLWidget *m_glWidget;
        QPoint m_dragOffset;

    public:
        PetMainWindow();
        ~PetMainWindow();

    protected:
        void mousePressEvent(QMouseEvent *ev) override;
        void mouseMoveEvent(QMouseEvent *ev) override;
    };

}