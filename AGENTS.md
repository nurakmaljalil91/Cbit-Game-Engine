# AGENTS.md

This file provides concise, high-signal context for AI agents working in this repo.
Keep it updated as the project evolves.

## Project Overview
- Name: Cbit-Game-Engine
- Type: 3D game engine (C++/CMake)
- Purpose: learn computer graphics and build a game engine from scratch
- Features: SDL2 window/input, OpenGL rendering, JSON-based data, ImGui editor UI

## Repo Layout
- `src/`: engine source code
- `lib/`: third-party libraries (vendored or submodules)
- `resources/`: assets used by examples/tests
- `examples/`: sample apps and usage demos
- `tests/`: unit/integration tests
- `docs/`: design notes and user documentation
- `scripts/`: helper scripts

## Build & Run (CMake)
Debug (Ninja example from README):
```
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B build-debug
cmake --build build-debug
```

Release (editor disabled):
```
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -S . -B build-release -DENABLE_EDITOR=OFF
cmake --build build-release
```

Notes:
- `ENABLE_EDITOR` toggles the ImGui editor.
- README uses `build-debug`/`build-release` as output dirs.

## Testing
If tests are configured:
```
ctest --test-dir build-debug
```

## Key Entry Points
- `src/main.cpp`: application entry point and game loop start
- `src/Application.h`: SDL setup, window, input, render loop, editor integration

## Coding Standards
- Language: C++20 (from `CMakeLists.txt`)
- Style: keep changes small and localized; prefer clear naming over comments
- Headers: include order should be consistent (project, third-party, std)
- Error handling: avoid exceptions unless already used in the subsystem
- Ownership: prefer RAII; avoid raw `new/delete` in new code

## Engine Architecture (High-Level)
- Application: owns Window, Input, SceneManager, ProjectManager, CameraManager, ShaderManager; runs update/render
- Window/Input: SDL2 window + OpenGL context; `Input` gathers SDL events
- Project & Scenes: `Project`/`ProjectManager` manage project metadata and scene list; `SceneManager` loads/saves scenes; `Scene` owns ECS
- ECS: `EntityComponentSystem` (entt registry) and `GameObject` wrapper; systems include `CameraSystem`, `LightingSystem`
- Cameras: `CameraManager` handles Editor/UI/Game3D/Game2D/Cutscene cameras; editor uses `OrbitCamera`, UI uses `UICamera`
- Graphics: shader management, textures, meshes, lighting, text rendering (OpenGL)
- Editor (optional): ImGui panels, offscreen FBO for game viewport, editor camera controls

## Rendering Pipeline (Current)
- `Application::_render` -> `SceneManager::render(cameraManager)` -> `Scene::render` -> `EntityComponentSystem::render`
- ECS render path uses shader `mesh_lighting` from `resources/shaders/mesh_lighting.vert/.frag`
- `CameraSystem::bindActiveCamera` sets view/projection based on the primary `CameraComponent`
- `LightingSystem::applyAllLights` pushes directional/point/spot light uniforms
- Draw loop currently renders `QuadComponent` and `CubeComponent` meshes; `TextureComponent` binds a `Texture`
- Editor scene view renders to an offscreen FBO and displays it via ImGui; uses ImGuizmo for transforms
- Text rendering uses `TextRenderer` with `resources/shaders/text.vert/.frag`

## ECS Components (Current)
- Identity: `TagComponent`, `IdComponent`
- Transform: `TransformComponent` (position, rotation in degrees, scale; matrix via quaternion)
- Camera: `CameraComponent` (type, primary flag, fov/near/far, orbital params, optional ortho)
- Lights: `DirectionalLightComponent`, `PointLightComponent`, `SpotLightComponent`
- Geometry: `QuadComponent` (MeshQuad), `CubeComponent` (CubeMesh)
- Material: `TextureComponent` (path + GPU texture)

## Serialization
- Scene data saved/loaded as JSON via `SceneSerializer` (RapidJSON), stored under `scenes/` in a project folder

## Assets & Resources
- `core/project/AssetManager` loads SDL textures, audio (SDL_mixer), fonts (TTF)
- Prefer loading assets from `resources/` using relative paths
- New formats should include a minimal loader and a small sample asset

## Editor Workflow (Current)
- Setup: ImGui context, docking, themes, fonts; editor config stored in `config/editor.ini`
- Panels: Game Objects, Scene (viewport + gizmo), Components, Console, Asset Manager, Profile, Scenes
- Scene panel renders active scene into an FBO, then draws the texture in ImGui
- Game objects: create entities, select via list, edit components via the Components panel
- Scenes panel: create/switch/delete scenes; project load triggers scene load
- Editor camera: OrbitCamera with mouse drag + scroll; only active when scene panel hovered

## Asset Formats (Current)
- Scenes: JSON (`project.json` + `scenes/*.json`) via `SceneSerializer`
- Shaders: `resources/shaders/*.vert` and `*.frag`
- Textures: `.png/.jpg/.jpeg` in `resources/images` (SDL_image); editor texture picker uses paths
- Audio: `.wav/.mp3/.ogg` in `resources/audio` (SDL_mixer)
- Fonts: `.ttf` in `resources/fonts`; editor also loads `.otf` in `resources/fonts`
- Models: `Model` supports `.obj` and JSON-based model data (see `core/mesh/Model`)

## Performance & Determinism
- Avoid per-frame heap allocations in hot paths
- Be explicit about frame timing and update order

## Logging & Diagnostics
- Use existing logging utilities; do not add `printf` unless there is no logger
- Add small debug counters for performance-critical changes

## Dependencies
- Current: SDL2, SDL2_image, SDL2_ttf, SDL2_mixer, OpenGL, GLEW, SOIL, glad, jsoncpp, RapidJSON, entt, Dear ImGui, stb
- Prefer adding dependencies via existing mechanisms (`lib/` or CMake)
- Document any new third-party dependency in `docs/` or `README.md`

## How To Change Things
- Respect existing patterns in the subsystem you touch
- If a change touches engine architecture, add or update docs
- Add or update tests when behavior changes or new APIs are introduced

## AI Agent Instructions
- Read `README.md` and any relevant docs before large refactors
- Ask before adding new dependencies or changing build options
- Keep edits minimal and aligned with existing style
- Avoid touching unrelated files

## Quick Checklist (Before Final Response)
- Build still succeeds (if you can run it)
- Tests pass (if you can run them)
- No unrelated files modified
- Changes explained with file paths and intent
