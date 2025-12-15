#pragma once

#include <QOpenGLWidget>

namespace miraipet::ui
{

    class PetGLWidget : public QOpenGLWidget
    {
        Q_OBJECT
    private:

    public:
        PetGLWidget(QWidget* parent = nullptr);

    protected:
        void initializeGL() override;
        void paintGL() override;
    };

}