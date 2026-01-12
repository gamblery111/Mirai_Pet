add_library(mirai_pet_mmd STATIC
    MMDLoader.cpp
    MMDModelDrawContext.cpp
)

target_include_directories(mirai_pet_mmd
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mirai_pet_mmd
    PUBLIC
        mirai_pet_engine
        mirai_GL
    PRIVATE
        Saba
        BulletAll
)
