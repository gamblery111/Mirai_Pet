#include "PetAppController.h"
#include "context/ViewerContext.h"
namespace miraipet::ui
{

    PetAppController::PetAppController()
    {
        m_upViewerCtx = std::make_unique<ViewCtx>();
    }

    PetAppController::~PetAppController()
    {
    }
}