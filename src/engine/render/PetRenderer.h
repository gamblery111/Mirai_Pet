#pragma once

#include "context/RenderContext.h"
#include <QOpenGLShaderProgram>

namespace miraipet::render
{

    class PetRenderer
    {
        using RenderCtx = context::RenderContext;

    private:
        RenderCtx* m_ctx; 
        QOpenGLShaderProgram* m_program{nullptr};
        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;
    
    public:
        PetRenderer(RenderCtx* ctx);
        ~PetRenderer();

        void Render();
        
    private:
        void Initialize();
        void CreateTriangle();
    };

}