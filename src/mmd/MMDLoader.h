#pragma once

#include <string>

#include "model/PetModelData.h"

namespace miraipet::MMD
{

    class MMDLoader
    {
    private:

    public:
        MMDLoader();
        ~MMDLoader();

        Model::PetModelData load(const std::string& _pmxPath, const std::string& mmdDataDir);
    };

} // namespace miraipet::MMD