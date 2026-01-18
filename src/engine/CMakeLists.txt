add_library(mirai_pet_engine STATIC
    model/PetModelData.cpp
    render/PetRenderer.cpp
    render/PetShaderManager.cpp
    runtime/PetRuntime.cpp
    context/ViewerContext.cpp
    view/Camera.cpp
    view/Light.cpp
    view/ShadowMap.cpp
)

target_include_directories(mirai_pet_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mirai_pet_engine
    PUBLIC
        Qt6::OpenGL
    PRIVATE
        Saba
        mirai_BaseUtils
        mirai_GL
)
