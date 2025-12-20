#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

namespace miraipet::render
{

    class PetShaderManager
    {
    private:
        QOpenGLFunctions_3_3_Core *m_gl;

    public:
        PetShaderManager(QOpenGLFunctions_3_3_Core *_gl);
        ~PetShaderManager();

        QOpenGLShaderProgram *CreateBasicShader();
        QOpenGLShaderProgram *CreateMMDShader();
    };

}