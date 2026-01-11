add_library(mirai_BaseUtils STATIC
    PathUtil.cpp
    UnicodeUtil.cpp
    FileUtil.cpp
)

target_include_directories(mirai_BaseUtils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mirai_BaseUtils
    PUBLIC
    PRIVATE
)

target_compile_definitions(mirai_BaseUtils PRIVATE UNICODE _UNICODE)
