#include "PetShaderManager.h"

namespace miraipet::render
{

    PetShaderManager::PetShaderManager(QOpenGLFunctions_3_3_Core *_gl)
        : m_gl(_gl)
    {
    }

    PetShaderManager::~PetShaderManager()
    {
    }

    QOpenGLShaderProgram *miraipet::render::PetShaderManager::CreateBasicShader()
    {
        auto *program = new QOpenGLShaderProgram();

        const char *vert = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
        )";

        const char *frag = R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(0.8, 0.3, 0.2, 1.0);
        }
        )";

        program->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
        program->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
        program->link();
        return program;
    }

}