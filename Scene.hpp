#pragma once
#include <vector>
#include <box2d/box2d.h>
#include "entt.hpp"


class Entity;

class Scene
{
private:
    std::vector<Entity> m_Entities;
    entt::registry m_Registry;
    b2World* m_PhysicsWorld;
public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template <typename... Components>
    auto GetAllEntitiesWith()
    {
        return m_Registry.view<Components...>();
    }


    void CreatePhysicsEntity(Entity entity);
    void OnStart();
    void OnUpdate(float ts);
    void OnStop();


    std::vector<Entity> GetEntities()
    {
        return m_Entities;
    }

    friend class Entity;
};
