#include "PetRuntime.h"

namespace miraipet::runtime
{
    PetRuntime::PetRuntime(const PetModel &model)
        : m_model(model)
    {
    }

    PetRuntime::~PetRuntime()
    {
    }

}