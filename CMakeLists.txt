cmake_minimum_required(VERSION 3.27)

project(cbit_game_framework)

set(CMAKE_CXX_STANDARD 11)

# set sdl2 path
set(SDL2_PATH vendors/SDL2-2.28.5/x86_64-w64-mingw32)
include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)
file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

# set glm path
set(GLM_PATH vendors/glm-0.9.9.8/glm)
include_directories(${GLM_PATH})

# set spdlog path
if (NOT TARGET spdlog)
    # Stand-alone build
    set(spdlog_DIR vendors/spdlog-1.12.0)
    find_package(spdlog REQUIRED)
endif ()

# set glad path
set(GLAD_PATH vendors/glad)
include_directories(${GLAD_PATH}/include)

# set rapidjson path
set(RAPIDJSON_PATH vendors/rapidjson-1.1.0)
include_directories(${RAPIDJSON_PATH}/include)

# set stb_image path
set(STB_IMAGE_PATH vendors/stb)
include_directories(${STB_IMAGE_PATH})

add_executable(cbit_game_framework
        src/main.cpp
        ${GLAD_PATH}/src/glad.c
        src/Game.cpp
        src/Game.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/Config.h
        src/ecs/Entity.cpp
        src/ecs/EntitiesManager.cpp
        src/core/Camera.cpp
        src/core/VertexArray.cpp
        src/core/Texture.cpp
        src/core/Mesh.cpp
        src/core/ShaderProgram.cpp
        src/scene/SceneManager.cpp
        src/scene/Scene.cpp
        src/utilities/Math.cpp
        src/utilities/ResourcesDirectory.cpp
        src/imgui/imgui_impl_sdl.cpp
        src/imgui/imgui_impl_opengl3.cpp
        src/imgui/imgui.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/ImGuiLayer.cpp
)

target_link_libraries(cbit_game_framework mingw32 SDL2main SDL2 spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)