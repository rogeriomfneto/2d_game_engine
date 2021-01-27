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
    for (auto& entity: this->entities)
        entity->Render();
}

Entity& EntityManager::AddEntity(std::string entityName) {
    Entity *entity = new Entity(*this, entityName);
    this->entities.push_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return this->entities;
}

unsigned int EntityManager::GetEntityCount() {
    return this->entities.size();
}