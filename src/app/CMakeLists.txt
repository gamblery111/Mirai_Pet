add_library(mirai_pet_app STATIC
    PetMainWindow.cpp
    PetGLWidget.cpp
    PetAppController.cpp
)

target_include_directories(mirai_pet_app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mirai_pet_app
    PUBLIC
        mirai_pet_engine
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)
