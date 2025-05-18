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

# Add Core sources
set(CORE_SOURCES
        src/core/AssetManager.cpp
        src/core/AssetManager.h
        src/core/Camera.cpp
        src/core/Camera.h
        src/core/Components.h
        src/core/EntityComponentSystem.cpp
        src/core/EntityComponentSystem.h
        src/core/GameObject.cpp
        src/core/GameObject.h
        src/core/Input.cpp
        src/core/Input.h
        src/core/Keyboard.h
        src/core/Locator.h
        src/core/Locator.cpp
        src/core/Mesh.cpp
        src/core/Mesh.h
        src/core/MeshQuad.cpp
        src/core/MeshQuad.h
        src/core/Model.cpp
        src/core/Model.h
        src/core/MouseButton.h
        src/core/Quad.cpp
        src/core/Quad.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h
        src/core/ShaderProgram.cpp
        src/core/ShaderProgram.h
        src/core/SplashScreen.cpp
        src/core/SplashScreen.h
        src/core/TextRenderer.cpp
        src/core/TextRenderer.h
        src/core/Texture.cpp
        src/core/Texture.h
        src/core/VertexArray.cpp
        src/core/VertexArray.h
        src/core/ShaderManager.cpp
        src/core/ShaderManager.h
        src/core/Window.cpp
        src/core/Window.h
        src/core/CubeMesh.cpp
        src/core/CubeMesh.h
        src/core/EditorCamera.cpp
        src/core/EditorCamera.h
        src/core/OrbitCamera.cpp
        src/core/OrbitCamera.h
        src/core/UICamera.cpp
        src/core/UICamera.h
        src/core/CameraManager.cpp
        src/core/CameraManager.h
        src/core/Lighting.h
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