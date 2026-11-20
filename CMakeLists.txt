cmake_minimum_required(VERSION 3.22.0)
project(Projet3D_PBF LANGUAGES C CXX)

include(FetchContent)

# GLM library
set(GLM_BUILD_LIBRARY OFF)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863  # GLM 1.0.1
)
FetchContent_MakeAvailable(glm)

# PBF static core library
add_library(pbf_core STATIC
    src/core/PBFSolver.cpp
)
target_include_directories(pbf_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core"
)
target_compile_features(pbf_core PUBLIC cxx_std_17)
target_link_libraries(pbf_core PUBLIC glm::glm)


# project executable target
add_executable(Projet3D_PBF src/main.cpp)
target_compile_features(Projet3D_PBF PRIVATE cxx_std_17)
target_link_libraries(Projet3D_PBF PRIVATE pbf_core)


# GLFW app target
option(PBF_BUILD_GLFW "Build the GLFW particle viewer" ON)
if(PBF_BUILD_GLFW)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_EXAMPLES OFF)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)

    # glad library target
    add_library(pbf_glad STATIC third_party/glad/src/gl.c)
    target_include_directories(pbf_glad PUBLIC third_party/glad/include)

    add_executable(pbf_glfw
        src/glfw/main.cpp
        src/glfw/CameraInput.cpp
        src/render/Camera.cpp
        src/render/Shader.cpp
        src/render/ParticleRender.cpp
    )

    target_include_directories(pbf_glfw PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    target_compile_definitions(pbf_glfw PRIVATE
        ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
    )
    target_link_libraries(pbf_glfw PRIVATE pbf_core glfw pbf_glad)
endif()


