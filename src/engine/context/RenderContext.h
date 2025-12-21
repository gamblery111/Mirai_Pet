#pragma once

namespace miraipet::render{class PetShaderManager;}
namespace miraipet::Model{class PetModelData;}

namespace miraipet::context {
       
    struct RenderContext {
        render::PetShaderManager* shaderManager = nullptr;
        float aspectRatio = 1.0f;  // 默认宽高比为1:1
        std::shared_ptr<Model::PetModelData> currentModelData = nullptr;
    };

}