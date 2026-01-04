#pragma once

#include <string>
#include <memory>

#include "model/PetModelData.h"

namespace miraipet::model{class PetModelData;}

namespace miraipet::MMD
{
    class MMDLoader
    {
    private:
        float m_fModelScale{12.0f}; // 模型缩放倍数
    public:
        MMDLoader();
        ~MMDLoader();

        std::shared_ptr<Model::PetModelData> load(const std::string& _pmxPath, const std::string& mmdDataDir);
    };

} // namespace miraipet::MMD