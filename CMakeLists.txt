cmake_minimum_required (VERSION 3.21)

# 执行 project 命令，同时定义项目名和使用的语言
project(Mirai_Pet LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################   Qt ###################
# 查找 Qt6 库 (核心, QML, 3D)
find_package(Qt6 REQUIRED COMPONENTS
    Core 
    Gui 
    Widgets
    OpenGL
    OpenGLWidgets
)
qt_standard_project_setup()

################## 第三方库 ##################
include(cmake/ThirdParty.cmake)

################## 子模块 ##################
add_subdirectory(src/engine)
add_subdirectory(src/mmd)
add_subdirectory(src/app)

qt_add_executable(${PROJECT_NAME}
    src/Mirai_Pet.cpp
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        mirai_pet_app

)