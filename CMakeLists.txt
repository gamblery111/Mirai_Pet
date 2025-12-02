cmake_minimum_required (VERSION 3.21)

# 执行 project 命令，同时定义项目名和使用的语言
project(Mirai_Pet LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################   Qt ###################
# 查找 Qt6 库 (核心, QML, 3D)
find_package(Qt6 REQUIRED COMPONENTS
    Quick
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

set(QT_QML_GENERATE_QMLLS_INI ON)
# 将 QML 文件加入资源系统
qt_add_qml_module(${PROJECT_NAME}
    URI "MyPet"  
    VERSION 1.0
    QML_FILES 
        qml/Main.qml
)

# 添加资源
qt_add_resources( ${PROJECT_NAME} "pictures.qrc" PREFIX "/" FILES resources/picture/pet.jpg)

# ========  Saba + Bullet 配置开始  ========

set(SABA_INCLUDE_DIR 
    "E:/Code/Open_Source_Code/saba/src"
    "E:/Code/Open_Source_Code/saba/src/Saba"
    "E:/Code/Open_Source_Code/saba/viewer"
    "E:/Code/Open_Source_Code/saba/external/gl3w/include"
)
set(SABA_LIBRARY     "E:/Code/Open_Source_Code/saba/build-msvc2022x64/src/Debug/Saba.lib")

set(BULLET_INCLUDE_DIR "E:/Code/Open_Source_Code/bullet3/build-msvc2022x64/Install_Path/include")
set(BULLET_LIB_DIR     "E:/Code/Open_Source_Code/bullet3/build-msvc2022x64/Install_Path/lib")

# 添加 include
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SABA_INCLUDE_DIR}
        ${BULLET_INCLUDE_DIR}
)

add_library(Bullet3Dynamics      UNKNOWN IMPORTED)
set_target_properties(Bullet3Dynamics PROPERTIES
    IMPORTED_LOCATION_DEBUG          "${BULLET_LIB_DIR}/Bullet3Dynamics_Debug.lib"
)

add_library(Bullet3Collision    UNKNOWN IMPORTED)
set_target_properties(Bullet3Collision PROPERTIES
    IMPORTED_LOCATION_DEBUG          "${BULLET_LIB_DIR}/Bullet3Collision_Debug.lib"
)

add_library(Bullet3Common      UNKNOWN IMPORTED)
set_target_properties(Bullet3Common PROPERTIES
    IMPORTED_LOCATION_DEBUG          "${BULLET_LIB_DIR}/Bullet3Common_Debug.lib"
)

add_library(LinearMath    UNKNOWN IMPORTED)
set_target_properties(LinearMath PROPERTIES
    IMPORTED_LOCATION_DEBUG          "${BULLET_LIB_DIR}/LinearMath_Debug.lib"
)
add_library(BulletSoftBody      UNKNOWN IMPORTED)
set_target_properties(BulletSoftBody PROPERTIES
    IMPORTED_LOCATION_DEBUG          "${BULLET_LIB_DIR}/BulletSoftBody_Debug.lib"
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Quick
        Qt6::Core
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
         ${SABA_LIBRARY}

        Bullet3Dynamics
        Bullet3Collision
        Bullet3Common
        LinearMath
        BulletSoftBody
        debug     "${BULLET_LIB_DIR}/BulletDynamics_Debug.lib"
        optimized "${BULLET_LIB_DIR}/BulletDynamics.lib"
        
        debug     "${BULLET_LIB_DIR}/BulletCollision_Debug.lib"
        optimized "${BULLET_LIB_DIR}/BulletCollision.lib"
)