# =========================
# 导入第三方静态库
# =========================
function(add_imported_static_lib name debug_lib release_lib)
    add_library(${name} STATIC IMPORTED)
    set_target_properties(${name} PROPERTIES
        IMPORTED_LOCATION_DEBUG   "${debug_lib}"
        IMPORTED_LOCATION_RELEASE "${release_lib}"
    )
endfunction()

# =========================
# Saba
# =========================
add_imported_static_lib(
    Saba
    "${CMAKE_SOURCE_DIR}/thirdParty/saba/lib/Debug/Saba.lib"
    "${CMAKE_SOURCE_DIR}/thirdParty/saba/lib/Release/Saba.lib"
)

target_include_directories(Saba INTERFACE
    "${CMAKE_SOURCE_DIR}/thirdParty"
    "${CMAKE_SOURCE_DIR}/thirdParty/saba"
)

# =========================
# Bullet 路径
# =========================
set(BULLET_ROOT "${CMAKE_SOURCE_DIR}/thirdParty/Bullet3")
set(BULLET_INCLUDE_DIR "${BULLET_ROOT}/include")
set(BULLET_LIB_DIR     "${BULLET_ROOT}/lib")

# =========================
# Bullet 头文件
# =========================
add_library(BulletHeaders  INTERFACE)

target_include_directories(BulletHeaders  INTERFACE
    "${BULLET_INCLUDE_DIR}"
)

# =========================
# 创建 Bullet 模块
# =========================
set(BULLET_LIBS
    BulletDynamics
    Bullet3Dynamics
    BulletCollision
    Bullet3Collision
    Bullet3Common
    LinearMath
    BulletSoftBody
)

foreach(lib IN LISTS BULLET_LIBS)
    add_imported_static_lib(
        ${lib}
        "${BULLET_LIB_DIR}/${lib}_Debug.lib"
        "${BULLET_LIB_DIR}/${lib}.lib"
    )

    target_link_libraries(${lib} INTERFACE BulletHeaders )
endforeach()

# ------------------------
# 创建聚合 target
# ------------------------
add_library(BulletAll INTERFACE)

target_link_libraries(BulletAll INTERFACE
    BulletDynamics
    Bullet3Dynamics
    BulletCollision
    Bullet3Collision
    Bullet3Common
    LinearMath
    BulletSoftBody
)