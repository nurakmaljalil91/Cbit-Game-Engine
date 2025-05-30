cmake_minimum_required(VERSION 3.28)

project(Cbit VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Set C++ standard to C++20 (For GoogleTest) / Can change to C++17 if needed

if (WIN32)
    # set spdlog path
    set(spdlog_PATH lib/spdlog)
    include_directories(${spdlog_PATH}/include)
    link_directories(${spdlog_PATH}/lib)

    # Set CMAKE_PREFIX_PATH to include spdlog_DIR
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${spdlog_PATH})

    find_package(spdlog REQUIRED)
else ()
    find_package(spdlog REQUIRED)
endif ()

# Platform‐specific SDL2 setup
if (WIN32)
    # SDL2
    set(SDL2_PATH ${CMAKE_SOURCE_DIR}/lib/SDL2)
    list(APPEND CMAKE_PREFIX_PATH ${SDL2_PATH})
    include_directories(${SDL2_PATH}/include)
    link_directories(${SDL2_PATH}/lib)
    file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

    # SDL2_image
    set(SDL2_IMAGE_PATH ${CMAKE_SOURCE_DIR}/lib/SDL_image)
    list(APPEND CMAKE_PREFIX_PATH ${SDL2_IMAGE_PATH})
    include_directories(${SDL2_IMAGE_PATH}/include)
    link_directories(${SDL2_IMAGE_PATH}/lib)
    file(COPY ${SDL2_IMAGE_PATH}/bin/SDL2_image.dll DESTINATION ${CMAKE_BINARY_DIR})

    # SDL2_ttf
    set(SDL2_TTF_PATH ${CMAKE_SOURCE_DIR}/lib/SDL_ttf)
    list(APPEND CMAKE_PREFIX_PATH ${SDL2_TTF_PATH})
    include_directories(${SDL2_TTF_PATH}/include)
    link_directories(${SDL2_TTF_PATH}/lib)
    file(COPY ${SDL2_TTF_PATH}/bin/SDL2_ttf.dll DESTINATION ${CMAKE_BINARY_DIR})

    # SDL2_mixer
    set(SDL2_MIXER_PATH ${CMAKE_SOURCE_DIR}/lib/SDL_mixer)
    list(APPEND CMAKE_PREFIX_PATH ${SDL2_MIXER_PATH})
    include_directories(${SDL2_MIXER_PATH}/include)
    link_directories(${SDL2_MIXER_PATH}/lib)
    file(COPY ${SDL2_MIXER_PATH}/bin/SDL2_mixer.dll DESTINATION ${CMAKE_BINARY_DIR})

    # we still call find_package to populate SDL2_INCLUDE_DIRS etc.
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

else ()
    # On Linux/Debian‐like systems, use system packages
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(OpenGL REQUIRED)

    include_directories(
            ${SDL2_INCLUDE_DIRS}
            ${SDL2_IMAGE_INCLUDE_DIRS}
            ${SDL2_TTF_INCLUDE_DIRS}
            ${SDL2_MIXER_INCLUDE_DIRS}
            ${OpenGL_INCLUDE_DIRS}
    )
endif ()

# set glm path
set(GLM_PATH lib/glm)
include_directories(${GLM_PATH}/include)

# set glad path
set(GLAD_PATH lib/glad)
include_directories(${GLAD_PATH}/include)

# set rapidjson path
set(RAPIDJSON_PATH lib/rapidjson)
include_directories(${RAPIDJSON_PATH}/include)

# set stb_image path
set(STB_IMAGE_PATH lib/stb)
include_directories(${STB_IMAGE_PATH})

# set entt path
include_directories(lib/entt)

# set simdjson path
set(SIMDJSON_PATH lib/simdjson)
include_directories(${SIMDJSON_PATH}/include)
link_directories(${SIMDJSON_PATH}/lib)

# set tinyobjloader path
set(TINYOBJLOADER_PATH lib/tinyobjloader)
include_directories(${TINYOBJLOADER_PATH}/include)
link_directories(${TINYOBJLOADER_PATH}/lib)

# set dirent path
set(DIRENT_PATH lib/dirent)
include_directories(${DIRENT_PATH}/include)

# set GoogleTest library
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Copy all files in the assets folder
file(GLOB ASSETS_FILES ${CMAKE_SOURCE_DIR}/examples/assets/*)
file(COPY ${ASSETS_FILES} DESTINATION ${CMAKE_BINARY_DIR}/assets)

# Copy all files in the resources folder
file(GLOB RESOURCES_FILES ${CMAKE_SOURCE_DIR}/resources/*)
file(COPY ${RESOURCES_FILES} DESTINATION ${CMAKE_BINARY_DIR}/resources)

# Copy all files in the config folder
file(GLOB CONFIG_FILES ${CMAKE_SOURCE_DIR}/resources/config/*)
file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/config)

# set core camera sources
set(CORE_CAMERA_SOURCES
        src/core/camera/Camera.cpp
        src/core/camera/Camera.h
        src/core/camera/CameraManager.cpp
        src/core/camera/CameraManager.h
        src/core/camera/EditorCamera.cpp
        src/core/camera/EditorCamera.h
        src/core/camera/OrbitCamera.cpp
        src/core/camera/OrbitCamera.h
        src/core/camera/UICamera.cpp
        src/core/camera/UICamera.h
)

# set core entity component system sources
set(CORE_ECS_SOURCES
        src/core/ecs/Components.h
        src/core/ecs/EntityComponentSystem.cpp
        src/core/ecs/EntityComponentSystem.h
        src/core/ecs/GameObject.cpp
        src/core/ecs/GameObject.h
)

set(CORE_GRAPHICS_SOURCES
        src/core/graphic/Lighting.h
        src/core/graphic/ShaderManager.cpp
        src/core/graphic/ShaderManager.h
        src/core/graphic/ShaderProgram.cpp
        src/core/graphic/ShaderProgram.h
        src/core/graphic/TextRenderer.cpp
        src/core/graphic/TextRenderer.h
        src/core/graphic/Texture.cpp
        src/core/graphic/Texture.h
        src/core/graphic/VertexArray.cpp
        src/core/graphic/VertexArray.h
)

# set core input sources
set(CORE_INPUT_SOURCES
        src/core/input/Input.cpp
        src/core/input/Input.h
        src/core/input/Keyboard.h
        src/core/input/MouseButton.h
)

set(CORE_LOCATOR_SOURCES
        src/core/locator/Locator.cpp
        src/core/locator/Locator.h
)

set(CORE_MESH_SOURCES
        src/core/mesh/CubeMesh.cpp
        src/core/mesh/CubeMesh.h
        src/core/mesh/Mesh.cpp
        src/core/mesh/Mesh.h
        src/core/mesh/MeshQuad.cpp
        src/core/mesh/MeshQuad.h
        src/core/mesh/Model.cpp
        src/core/mesh/Model.h
        src/core/mesh/Quad.cpp
        src/core/mesh/Quad.h
)

set(CORE_PROJECT_SOURCES
        src/core/project/AssetManager.cpp
        src/core/project/AssetManager.h
        src/core/project/Project.cpp
        src/core/project/Project.h
        src/core/project/ProjectManager.cpp
        src/core/project/ProjectManager.h
        src/core/project/Scene.cpp
        src/core/project/Scene.h
        src/core/project/SceneManager.cpp
        src/core/project/SceneManager.h
        src/core/project/SceneSerializer.cpp
        src/core/project/SceneSerializer.h
)

set(CORE_SPLASH_SOURCES
        src/core/splash/SplashScreen.cpp
        src/core/splash/SplashScreen.h
)

set(CORE_WINDOW_SOURCES
        src/core/window/Window.cpp
        src/core/window/Window.h
)

# Add Core sources
set(CORE_SOURCES
        ${CORE_CAMERA_SOURCES}
        ${CORE_ECS_SOURCES}
        ${CORE_GRAPHICS_SOURCES}
        ${CORE_INPUT_SOURCES}
        ${CORE_LOCATOR_SOURCES}
        ${CORE_MESH_SOURCES}
        ${CORE_PROJECT_SOURCES}
        ${CORE_WINDOW_SOURCES}
        ${CORE_SPLASH_SOURCES}
)

# Add ecs sources
set(ECS_SOURCES
        src/ecs/Component.h
        src/ecs/EntitiesManager.h
        src/ecs/EntitiesManager.cpp
        src/ecs/Entity.cpp
        src/ecs/Entity.h
        src/ecs/EntityManager.cpp
        src/ecs/EntityManager.h
)

# Add editor
set(EDITOR_SOURCES
        src/editor/Editor.cpp
        src/editor/Editor.h
        src/editor/EditorLogSink.cpp
        src/editor/EditorLogSink.h
        src/editor/EditorMainMenuBar.cpp
        src/editor/EditorMainMenuBar.h

        src/editor/ProfilePanel.cpp
        src/editor/ProfilePanel.h
)

# Add Imgui sources
set(IMGUI_SOURCES
        src/imgui/imgui_impl_sdl2.cpp
        src/imgui/imgui_impl_opengl3.cpp
        src/imgui/imgui.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/ImGuiFileDialog.cpp
        src/imgui/ImGuiFileDialog.h
)

# Add Utilities sources
set(UTILITIES_SOURCES
        src/utilities/AssetsManager.cpp
        src/utilities/AssetsManager.h
        src/utilities/BuildGenerator.cpp
        src/utilities/BuildGenerator.h
        src/utilities/LocalMachine.cpp
        src/utilities/LocalMachine.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/utilities/Math.cpp
        src/utilities/Math.h
        src/utilities/Random.cpp
        src/utilities/Random.h
        src/utilities/ResourcesDirectory.cpp
        src/utilities/ResourcesDirectory.h
        src/utilities/Singleton.h
        src/utilities/UUIDGenerator.cpp
        src/utilities/UUIDGenerator.h
        src/utilities/DateTime.cpp
        src/utilities/DateTime.h
        src/utilities/SmartPointer.h
        src/utilities/JsonTemplate.h
)

# Add example sources
set(EXAMPLE_SOURCES
        examples/src/scenes/CubeScene.cpp
        examples/src/scenes/CubeScene.h
        examples/src/scenes/CubeTextureScene.cpp
        examples/src/scenes/CubeTextureScene.h
        examples/src/scenes/MeshScene.cpp
        examples/src/scenes/MeshScene.h
        examples/src/scenes/RectangleCameraScene.cpp
        examples/src/scenes/RectangleCameraScene.h
        examples/src/scenes/RectangleScene.cpp
        examples/src/scenes/RectangleScene.h
        examples/src/scenes/SimpleScene.cpp
        examples/src/scenes/SimpleScene.h
        examples/src/scenes/TriangleScene.cpp
        examples/src/scenes/TriangleScene.h
        examples/src/main.cpp
)

# Add Test sources
set(TESTS_SOURCES
        tests/SimpleTest.cpp
)

option(ENABLE_EDITOR "Enable ImGui-based in-game editor (only in dev builds)" ON)

# Create a shared library
add_library(Cbit SHARED
        ${GLAD_PATH}/src/glad.c
        ${CORE_SOURCES}
        ${ECS_SOURCES}
        ${EDITOR_SOURCES}
        ${IMGUI_SOURCES}
        ${UTILITIES_SOURCES}
        src/Application.cpp
        src/Application.h
        src/Config.h
)

# Create an executable for the example
add_executable(CbitApplication
        ${EXAMPLE_SOURCES}
)

# Create an executable for the main game engine application
add_executable(CbitGameEngine
        src/main.cpp
)

# Only compile & link in the editor sources when ENABLE_EDITOR=ON
if (ENABLE_EDITOR)
    target_sources(CbitApplication PRIVATE ${EDITOR_SOURCES})
    target_compile_definitions(CbitApplication PRIVATE ENABLE_EDITOR)
    target_compile_definitions(CbitGameEngine PRIVATE ENABLE_EDITOR)
    # Define it for your shared lib
    target_compile_definitions(Cbit PRIVATE ENABLE_EDITOR)
endif ()

# Embed Windows .ico via a .rc resource
if (WIN32)
    # Path to your .rc file (adjust if you placed it elsewhere)
    set(APP_ICON_RC "${CMAKE_CURRENT_SOURCE_DIR}/resources/branding/cbit.rc")

    # Tell CMake to compile and link it
    target_sources(CbitApplication PRIVATE ${APP_ICON_RC})

    # Ensure we link gdi32 on MinGW for icon resources
    target_link_libraries(CbitApplication PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:gdi32>
    )
endif ()

if (WIN32)
    target_link_libraries(Cbit PRIVATE
            mingw32)
    target_link_libraries(CbitApplication PRIVATE
            mingw32)
    target_link_libraries(CbitGameEngine PRIVATE
            mingw32)
endif ()
# Link SDL2 library for the library
target_link_libraries(Cbit PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        simdjson
        spdlog::spdlog
        $<$<BOOL:${MINGW}>:ws2_32>
        pthread
)

# Link SDL2 library for the executable
target_link_libraries(CbitApplication PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        simdjson
        spdlog::spdlog
        $<$<BOOL:${MINGW}>:ws2_32>
        pthread
        Cbit
)

# Link SDL2 library for the executable
target_link_libraries(CbitGameEngine PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        simdjson
        spdlog::spdlog
        $<$<BOOL:${MINGW}>:ws2_32>
        pthread
        Cbit
)

if (WIN32)
    # Set linker flags for console application
    set_target_properties(CbitApplication PROPERTIES
            LINK_FLAGS "-mconsole"
    )

    # Set linker flags for console application
    set_target_properties(CbitGameEngine PROPERTIES
            LINK_FLAGS "-mconsole"
    )
endif ()

# Specify the include directories
target_include_directories(Cbit PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/Cbit>
)

# Set the C++ standard
set_target_properties(Cbit PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)

# Export the targets to a script
install(TARGETS Cbit
        EXPORT CbitTargets
        RUNTIME DESTINATION bin           # for DLLs
        LIBRARY DESTINATION lib           # for shared libraries
        ARCHIVE DESTINATION lib)          # for static libraries and import libraries

# Install core header and source files to include/cbit2d/core
file(GLOB CORE_FILES
        "src/core/*.h"
        "src/core/*.cpp")
install(FILES ${CORE_FILES} DESTINATION include/Cbit/core)

# Install utilities header and source files to include/cbit2d/utilities
file(GLOB UTILITIES_FILES
        "src/utilities/*.h"
        "src/utilities/*.cpp")
install(FILES ${UTILITIES_FILES} DESTINATION include/Cbit/utilities)

# Install ecs header and source files to include/cbit2d/ecs
file(GLOB ECS_FILES
        "src/ecs/*.h"
        "src/ecs/*.cpp")
install(FILES ${ECS_FILES} DESTINATION include/Cbit/ecs)

# Install imgui header and source files to include/cbit2d/imgui
file(GLOB IMGUI_FILES
        "src/imgui/*.h"
        "src/imgui/*.cpp")
install(FILES ${IMGUI_FILES} DESTINATION include/Cbit/imgui)

# Install all header files to include/cbit2d
file(GLOB HEADER_FILES
        "src/*.h"
        "src/*.cpp")
install(FILES ${HEADER_FILES} DESTINATION include/Cbit)

# Create and install a package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CbitConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/CbitConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/Cbit"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/CbitConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CbitConfigVersion.cmake"
        DESTINATION lib/cmake/Cbit)

enable_testing()

add_executable(
        CbitTest
        ${TESTS_SOURCES}
)
target_link_libraries(
        CbitTest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(CbitTest)