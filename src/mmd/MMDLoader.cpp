#include "MMDLoader.h"

#include "saba/Model/MMD/PMXModel.h"

namespace miraipet::MMD
{

    MMDLoader::MMDLoader()
    {
    }

    MMDLoader::~MMDLoader()
    {
    }

    Model::PetModelData MMDLoader::load(const std::string &_pmxPath, const std::string &mmdDataDir)
    {
        saba::PMXModel pmx;
        pmx.Load(_pmxPath, mmdDataDir);

        Model::PetModelData model;

        return model;
    }

}