#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

namespace miraipet::render
{

    class PetShaderManager
    {
    private:
        QOpenGLShaderProgram *m_mmdShader;
        QOpenGLTexture *m_toonTexture;

    public:
        PetShaderManager();
        ~PetShaderManager();

        QOpenGLShaderProgram *GetMMDShader();
        QOpenGLTexture *GetToonTexture();

    private:
        void InitMMDShader();
        void CreateToonTexture();
    };

}