#pragma once

#include <QOpenGLShaderProgram>

namespace miraipet::render
{

    class PetShaderManager
    {
    public:
        PetShaderManager();
        ~PetShaderManager();

        QOpenGLShaderProgram *CreateBasicShader();
        QOpenGLShaderProgram *CreateMMDShader();
    };

}