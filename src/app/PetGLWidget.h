#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <memory>

namespace miraipet::ui
{

    class PetAppController;

    class PetGLWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
    {
        // Q_OBJECT
    private:
        std::unique_ptr<PetAppController> m_controller;

    public:
        PetGLWidget(QWidget* parent = nullptr);
        ~PetGLWidget();

    protected:
        void initializeGL() override;
        void paintGL() override;
    };

}