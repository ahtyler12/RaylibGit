#include "EntityManager.h"

EntityManager::~EntityManager()
{
    //Clear out both lists of entities
    Entities.clear();
    WaitingEntities.clear();
}

void EntityManager::CreateEntity(std::shared_ptr<Entity> _entity)
{
    //Add Entity to be added next frame
    WaitingEntities.push_back(_entity);
}

void EntityManager::AddEntity(std::shared_ptr<Entity> _entity)
{
    //Add Entity to active list of entities
    Entities.push_back(_entity);
}

void EntityManager::UpdateEntities()
{
    //Add any queued entities to the list of Entities
    for( unsigned int i = 0; i < Entities.size(); i++)
    {
        AddEntity(WaitingEntities[i]);
    }
    //Clear out queue
    WaitingEntities.clear();

    //Update Entities
    for( unsigned int i = 0; i < Entities.size(); i++)
    {
        Entities.at(i)->Update();
    }
}

void EntityManager::DrawEntities()
{
    for( unsigned int i = 0; i < Entities.size(); i++)
    {
        Entities.at(i)->Draw();
    }
}
