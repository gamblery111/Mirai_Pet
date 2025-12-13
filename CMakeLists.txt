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

# 处理所有 include path, output path, MOC/RCC/UIC
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    src/PetModelController.cpp
    src/PetRenderer.cpp
    src/PetWindow.cpp
    src/Mirai_Pet.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# ========  Saba + Bullet 配置  ========
include(cmake/ThirdParty.cmake)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets

        Saba
        BulletAll
)