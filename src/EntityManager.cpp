#include <iostream>
#include "./EntityManager.h"

void EntityManager::ClearData() {
    for (auto& entity: this->entities)
        entity->Destroy();
}

bool EntityManager::HasNoEntities() {
    return this->entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
    for (auto& entity: this->entities)
        entity->Update(deltaTime);
}

void EntityManager::Render() {
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        auto entitiesToRender = GetEntitiesByLayer(static_cast<LayerType>(layerNumber));
        for (auto& entity: entitiesToRender)
            entity->Render();
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    this->entities.push_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return this->entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (auto& entity: entities) {
        if (entity->layer == layer)
            selectedEntities.push_back(entity);
    }
    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount() {
    return this->entities.size();
}

void EntityManager::ListAllEntities() {
    for (auto entity: this->entities) {
        std::cout << "Entity Name: " << entity->name << std::endl;
        entity->ListAllComponents();
        std::cout << std::endl;
    }
}