/**
 * @file   EntitiesManager.cpp
 * @brief  Implementation file for the EntitiesManager class.
 * @detail This file contains the implementation of the EntitiesManager class which is responsible for managing
 * @author Nur Akmal bin Jalil
 * @date   2025-05-10
 */

#include "EntitiesManager.h"
#include <algorithm>

void EntitiesManager::Add(const std::shared_ptr<Entity>& entity) {
    newEntities.push_back(entity); // add entity to the entities
}

void EntitiesManager::Add(std::vector<std::shared_ptr<Entity> > &otherEntity) {
    newEntities.insert(newEntities.end(), otherEntity.begin(), otherEntity.end());
}

void EntitiesManager::Start() {
}

void EntitiesManager::HandleEvents() const
{
    for (const auto &e: entities) {
        e->HandleEvents(); // Handle events here
    }
}

void EntitiesManager::Update(const float delta_time) {
    InitNewEntities();
    QueueForRemoval();
    for (const auto &e: entities) {
        e->Update(delta_time); // update all the entities
    }
}

void EntitiesManager::Render(ShaderProgram *shader) const
{
    // TODO: Draw based on entities layer order
    for (const auto &e: entities) {
        e->Render(shader); // Render all the entities
    }
}

void EntitiesManager::Clear() const
{
    for (const auto &e : entities)
    {
        e->Clear(); // Clear all the entities
    }
}

bool EntitiesManager::Have(const std::shared_ptr<Entity>& entity) {
    if (std::ranges::find(entities, entity) != entities.end()) {
        return true;
    } else {
        return false;
    }
}

void EntitiesManager::InitNewEntities() {
    entities.insert(entities.end(), newEntities.begin(), newEntities.end());
    // combine the new entities with the original entities

    newEntities.clear(); // clear the object inside the new entities
}

void EntitiesManager::QueueForRemoval() {
    auto entity_iter = entities.begin(); // create iterator for the entity

    while (entity_iter != entities.end()) {
        if (const auto ent = *entity_iter; ent->IsQueuedForRemoval()) {
            entity_iter = entities.erase(entity_iter);
        } else {
            ++entity_iter;
        }
    }
}

std::vector<std::shared_ptr<Entity> > EntitiesManager::GetEntities() const {
    return entities;
}

void EntitiesManager::ImGuiShowEntities(bool *p_open) {
    // ImGui::Begin("Entity Manager Menu");
    // ImGui::Text("Entites Count = %d", entities.size());
    // for (const auto &e : entities)
    // {
    //     ImGui::Text(e->gameObject.name);
    //     ImGui::Text(e->gameObject.tag);
    //     }
    // ImGui::End();
}
