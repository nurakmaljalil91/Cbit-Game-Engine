#include "Mesh.h"

Mesh::Mesh() : vertex_array(nullptr),
               radius(0.0f),
               specular_power(100.0f) {}

Mesh::~Mesh() {}

bool Mesh::Load(const std::string &filename, Renderer *renderer)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        SDL_Log("[ERROR] File not found : Mesh %s", filename.c_str());
        return false;
    }

    std::stringstream file_stream;
    file_stream << file.rdbuf();
    std::string contents = file_stream.str();
    rapidjson::StringStream json_str(contents.c_str());
    rapidjson::Document document;
    document.ParseStream(json_str);

    if (!document.IsObject())
    {
        SDL_Log("[ERROR] Mesh %s is not valid json", filename.c_str());
        return false;
    }

    int version = document["version"].GetInt();

    // Check version
    if (version != 1)
    {
        SDL_Log("[ERROR] Mesh % not version 1", filename.c_str());
        return false;
    }

    shader_name = document["shader"].GetString();

    // Skip the vertex format/ shader for now
    // (This is changed in a later chapter's code)
    size_t vertex_size = 8;

    // Load textures
    const rapidjson::Value& textures = document["textures"];
    if(!textures.IsArray() || textures.Size() < 1){
        SDL_Log("Mesh % ha no textures, there should be at least one", filename.c_str());
        return false;
    }

    specular_power = static_cast<float>(document["specularPower"].GetDouble());

    for(rapidjson::SizeType i = 0; i < textures.Size(); i++){
        // Is this texture already loaded?
        std::string texture_name = textures[i].GetString();
        //Texture* t = renderer->
    }

}
