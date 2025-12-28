#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <memory>

namespace miraipet::ui
{

    class PetAppController;

    class PetGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
    private:
        std::unique_ptr<PetAppController> m_controller;

    public:
        explicit PetGLWidget(QWidget *parent = nullptr);
        ~PetGLWidget();

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
    };

}