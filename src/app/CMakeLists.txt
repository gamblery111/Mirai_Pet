add_library(mirai_pet_engine STATIC
    PetMainWindow.cpp
    PetGLWidget.cpp
)

target_include_directories(mirai_pet_engine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)
target_link_libraries(mirai_pet_engine
    PUBLIC
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)