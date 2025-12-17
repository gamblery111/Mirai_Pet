#pragma once
#include <QOpenGLFunctions_3_3_Core>

namespace miraipet::render{class PetShaderManager;}

namespace miraipet::context {
       
    struct RenderContext {
        QOpenGLFunctions_3_3_Core* gl = nullptr;
        render::PetShaderManager* shaderManager = nullptr;
    };

}