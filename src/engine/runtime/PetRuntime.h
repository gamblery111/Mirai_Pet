#pragma once

#include "model/PetModelData.h"

namespace miraipet::runtime
{

    class PetRuntime
    {
        using PetModel = Model::PetModelData;
    private:
        const PetModel& m_model;
        float m_time = 0.0f;

    public:
        PetRuntime(const PetModel& model);
        ~PetRuntime();
    };

}