add_library(mirai_GL STATIC
    GLSLUtil.cpp
    GLShaderUtil.cpp
)

target_include_directories(mirai_GL
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../../thirdParty//fcpp
)

target_link_libraries(mirai_GL
    PUBLIC
        Qt6::OpenGL
        mirai_BaseUtils
    PRIVATE
        Saba
)
