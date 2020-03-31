#include "Renderer.h"


Renderer::Renderer() : 
                                  sprite_shader(nullptr),
                                  mesh_shader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float _screen_width, float _screen_height)
{
    screen_width = _screen_width;
    screen_height = _screen_height;

    // Set OpenGL attributes
    // Use the core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a color buffer with 8-bits per RGBA channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    window = SDL_CreateWindow("Default", 100, 100,
                              static_cast<int>(screen_width), static_cast<int>(screen_height), SDL_WINDOW_OPENGL);

    if (!window)
    {
        SDL_Log("[ERROR] Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    context = SDL_GL_CreateContext(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) // Initialize OpenGL loader
    {
        SDL_Log("[ERROR] Failed to initialize GLEW.");
        return false;
    }

    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();

    // Make sure we can create/compile shaders
    if (!Load_Shaders())
    {
        SDL_Log("[ERROR] Failed to load shaders.");
        return false;
    }

    // Create quad for drawing sprites
    void Create_Sprite_Vertices();

    return true; // Initialize success
}

void Renderer::Shutdown()
{
    delete sprite_vertices;
    sprite_shader->Unload();
    delete sprite_shader;
    mesh_shader->Unload();
    delete mesh_shader;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void Renderer::Unload_Data()
{
    // Destroy textures
    for (auto i : textures)
    {
        i.second->Unload();
        delete i.second;
    }
    textures.clear();

    // Destroy meshes
    // for (auto i : meshes)
    // {
    //     i.second->Unload();
    //     delete i.second;
    // }
    // meshes.clear();
}

void Renderer::Render()
{
    // Set the clear color to light grey
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw mesh components
    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // Set the mesh shader active
    mesh_shader->Set_Active();
    // Update view-projection matrix
    mesh_shader->Set_Matrix_Uniform("view_projection", view * projection);
    // Update lighting uniforms
    Set_Light_Uniforms(mesh_shader);
    // FIXME: How to mesh component
    // for (auto mc : mMeshComps)
    // {
    // 	mc->Draw(mMeshShader);
    // }
    // Draw all sprite and components
    // disable depth buffering
    glDisable(GL_DEPTH_TEST);
    // Enable alpha blending on the color buffer
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // Set shader/vao as active
    sprite_shader->Set_Active();
    sprite_vertices->SetActive();

    // FIXME: How to sprite components
    // for (auto sprite : mSprites)
    // {
    // 	sprite->Draw(mSpriteShader);
    // }

    // Swap the buffers
    SDL_GL_SwapWindow(window);
}

Texture *Renderer::Get_Texture(const std::string &fileName)
{
    Texture *tex = nullptr;
    auto iter = textures.find(fileName);
    if (iter != textures.end())
    {
        tex = iter->second;
    }
    else
    {
        tex = new Texture();
        if (tex->Load(fileName))
        {
            textures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

bool Renderer::Load_Shaders()
{
    // Create sprite shader
    sprite_shader = new Shader();
    if (!sprite_shader->Load("../src/shader/Sprite.vert", "../src/shader/Sprite.frag"))
    {
        return false;
        std::cout << "something happen here! \n";
    }

    sprite_shader->Set_Active();
    // Set the view projection matrix
    Matrix4 view_projection = Matrix4::CreateSimpleViewProj(screen_width, screen_height);
    sprite_shader->Set_Matrix_Uniform("view_projection", view_projection);

    // Create basic mesh shader
    mesh_shader = new Shader();
    if (!mesh_shader->Load("../src/shader/Phong.vert", "../src/shader/Phong.frag"))
    {
        return false;
    }

    mesh_shader->Set_Active();
    // Set view projection matrix
    view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
                                               screen_width,
                                               screen_height, 25.0f, 10000.0f);
    mesh_shader->Set_Matrix_Uniform("view_projection", view * projection);
    return true;
}

void Renderer::Create_Sprite_Vertices()
{
    float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,  // top right
        0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    sprite_vertices = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::Set_Light_Uniforms(Shader *_shader)
{
    // Camera position is from inverted view
    Matrix4 inverted_view = view;
    inverted_view.Invert();
    _shader->Set_Vector_Uniform("camera_Position", inverted_view.GetTranslation());
    // Ambient light
    _shader->Set_Vector_Uniform("Ambient_Light", ambient_light);
    // Directional Light
    _shader->Set_Vector_Uniform("Directional_Light.direction", direction_light.direction);
    _shader->Set_Vector_Uniform("Directional_Light.diffuse_color", direction_light.diffuse_color);
    _shader->Set_Vector_Uniform("Directional_Light.specular_color", direction_light.specular_color);
}
