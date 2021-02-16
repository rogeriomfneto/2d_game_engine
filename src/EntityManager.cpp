#include <iostream>
#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

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

// std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
//     ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
//     for (auto& entity: entities) {
//         if (entity->name.compare(myEntity.name) == 0 || entity->name.compare("Tile") == 0) continue;
//         if (entity->HasComponent<ColliderComponent>()) {
//             ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
//             if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider))
//                 return otherCollider->colliderTag;
//         }
//     }

//     return std::string();
// }


CollisionType EntityManager::CheckCollisions() const {
    for (auto& thisEntity: entities) {
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (auto& thatEntity: entities) {
                if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("ENEMY") == 0
                        ) {
                            return PLAYER_ENEMY_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("PROJECTILE") == 0
                        ) {
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("ENEMY") == 0 &&
                            thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0
                        ) {
                            return ENEMY_PROJECTILE_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0
                        ) {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
}
