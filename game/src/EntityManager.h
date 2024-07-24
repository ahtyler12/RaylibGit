#include "Entity.h"
#include <vector>
#include <memory>
#pragma once


class EntityManager
{
private:
    std::vector<std::shared_ptr<Entity>> Entities;
    std::vector<std::shared_ptr<Entity>> WaitingEntities;
public:
    EntityManager(/* args */);
    ~EntityManager();

    void CreateEntity(std::shared_ptr<Entity> _entity);
    void DestroyEntity();
    void AddEntity(std::shared_ptr<Entity> _entity);
    void UpdateEntities();
    void DrawEntities();
};
