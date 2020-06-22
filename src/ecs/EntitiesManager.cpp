#include "EntitiesManager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"
void EntitiesManager::Add(std::shared_ptr<Entity> entity)
{
    newEntities.push_back(entity); // add entity to the entities
}

void EntitiesManager::Add(std::vector<std::shared_ptr<Entity>> &otherEntity)
{
    newEntities.insert(newEntities.end(), otherEntity.begin(), otherEntity.end());
}

void EntitiesManager::Start() {}

void EntitiesManager::HandleEvents()
{
    for (const auto &e : entities)
    {
        e->HandleEvents(); // Handle events here
    }
}

void EntitiesManager::Update(float delta_time)
{
    InitNewEntities();
    QueueForRemoval();
    for (const auto &e : entities)
    {
        e->Update(delta_time); // update all the entities
    }
}

void EntitiesManager::Render(ShaderProgram *shader)
{
    //std::cout << "Begin" << std::endl;
    // TODO: Draw based on entities layer order
    for (const auto &e : entities)
    {
        e->Render(shader); // Render all the entities
    }
}

void EntitiesManager::Clear()
{
    // for (const auto &e : entities)
    // {
    //     e->Clear(); // Clear all the entities
    // }
}

bool EntitiesManager::Have(std::shared_ptr<Entity> entity)
{
    if (std::find(entities.begin(), entities.end(), entity) != entities.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EntitiesManager::InitNewEntities()
{
    entities.insert(entities.end(), newEntities.begin(), newEntities.end()); // combine the new entities with the original entities

    newEntities.clear(); // clear the oject inside the new entities
}

void EntitiesManager::QueueForRemoval()
{
    auto entity_iter = entities.begin(); // create iterator for the entity

    while (entity_iter != entities.end())
    {

        auto ent = *entity_iter;

        if (ent->IsQueuedForRemoval())
        {
            entity_iter = entities.erase(entity_iter);
        }
        else
        {
            ++entity_iter;
        }
    }
}

std::vector<std::shared_ptr<Entity>> EntitiesManager::GetEntities() const
{
    return entities;
}

void EntitiesManager::ImGuiShowEntities(bool* p_open)
{
    // ImGui::Begin("Entity Manager Menu");
    // ImGui::Text("Entites Count = %d", entities.size());
    // for (const auto &e : entities)
    // {
    //     ImGui::Text(e->gameObject.name);
    //     ImGui::Text(e->gameObject.tag);
    //     }
    // ImGui::End();
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close"))
                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left
        static int selected = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < entities.size(); i++)
            {
                char label[128];
                sprintf(label, "MyObject %d", entities[i]->gameObject.name);
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            if (ImGui::Button("Revert"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
            }
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}