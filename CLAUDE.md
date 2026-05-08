# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```powershell
# Debug build (Ninja)
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B build-debug
cmake --build build-debug

# Release build (editor disabled)
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -S . -B build-release -DENABLE_EDITOR=OFF
cmake --build build-release

# Run tests
ctest --test-dir build-debug

# Run the engine
./build-debug/CbitGameEngine
```

`ENABLE_EDITOR=ON` (default) includes ImGui editor panels and FBO viewport. Set `OFF` for release/headless builds.

## Architecture

### Ownership & Lifecycle

`Application` is the root owner. It constructs and holds:
- `Window` → SDL2 window + OpenGL context
- `Input` → SDL event polling
- `ShaderManager` → loads/caches GLSL programs from `resources/shaders/`
- `CameraManager` → tracks Editor/Game/UI/Cutscene cameras
- `ProjectManager` → project metadata and disk I/O
- `SceneManager` → owns the active `Scene`; routes `update`/`render`

Each frame: `Application::_update` → `SceneManager::update` → `Scene::update` → `EntityComponentSystem::update`, then `Application::_render` → `SceneManager::render(cameraManager)` → `Scene::render` → ECS render systems.

### ECS

Built on **entt**. `EntityComponentSystem` wraps `entt::registry`. `GameObject` is a thin handle (entity + registry pointer) used to attach/query components. Systems (`CameraSystem`, `LightingSystem`) iterate the registry directly.

The two render systems run in order inside `EntityComponentSystem::render`:
1. `CameraSystem::bindActiveCamera` — pushes view/projection to the active shader
2. `LightingSystem::applyAllLights` — pushes directional/point/spot uniforms
3. Draw loop renders `QuadComponent`/`CubeComponent`; `TextureComponent` binds its GPU texture

### Scene Serialization

`SceneSerializer` (RapidJSON) reads/writes `scenes/<name>.json` inside a project folder. All ECS components that should persist must have explicit serialize/deserialize branches in `SceneSerializer`. When adding a new component, update both directions.

### Shader Loading

`ShaderManager::loadShader(name, vertPath, fragPath)` compiles and caches. Shaders are referenced by name string everywhere else. The primary rendering shader is `mesh_lighting` (`resources/shaders/mesh_lighting.vert/.frag`). Text uses `text`, UI uses `default`.

### Editor (ENABLE_EDITOR=ON)

`Editor` initializes ImGui docking, loads fonts, and sets up panels. The **Scene panel** renders the active scene into an offscreen FBO, then blits the texture into an ImGui image — this is why `Application::_render` has a separate FBO path. ImGuizmo draws transform gizmos over the same texture. Editor config persists to `resources/config/editor.ini`.

### Camera Types

`CameraManager` owns multiple camera slots. `CameraComponent` has a `type` field (`Editor`, `Game`, `UI`) and an `isPrimary` flag — `CameraSystem` binds the first primary camera it finds per type. `OrbitCamera` is used by the editor; `UICamera` is orthographic for 2D overlays.

## Key Files

| File | Role |
|------|------|
| `src/Application.h/.cpp` | Main loop, system wiring |
| `src/core/ecs/EntityComponentSystem.h` | entt registry wrapper, render/update dispatch |
| `src/core/ecs/Components.h` | All component structs (single header) |
| `src/core/ecs/Systems.h` | CameraSystem, LightingSystem |
| `src/core/project/SceneSerializer.h` | JSON save/load for all components |
| `src/core/graphic/ShaderManager.h` | Shader compilation and caching |
| `src/core/camera/CameraManager.h` | Multi-camera management |
| `src/editor/Editor.h` | ImGui editor bootstrap |
| `src/Config.h` | Global constants (window size, default FOV, shader names) |

## Adding a New Component

1. Declare the struct in `src/core/ecs/Components.h`
2. Add serialize/deserialize in `src/core/project/SceneSerializer.cpp`
3. If it needs per-frame logic, add iteration in `EntityComponentSystem::update` or a new system in `Systems.h`
4. If it needs editor editing, add a UI block in the Components panel (`src/editor/panels/`)

## Coding Standards

- C++20; RAII ownership; no raw `new/delete` in new code
- Use logging macros (`LOG_INFO`, `LOG_WARN`, `LOG_ERROR`) from `src/utilities/Logger.h` — no `printf`
- Include order: project headers → third-party → standard library
- Avoid per-frame heap allocations in hot paths
- Error handling: avoid exceptions unless already used in the subsystem being touched
