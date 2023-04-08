#pragma once
#include "entt.hpp"
#include "Scene.hpp"

class Scene;

class Entity
{
public:
    Entity() = default;

    Entity(entt::entity handle, Scene* scene)
    {
        this->m_Handle = handle;
        this->m_Scene = scene;
    }

    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

    template<typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args);

    template<typename T>
    T& GetComponent();

    template<typename T>
    bool HasComponent();

    template<typename T>
    void RemoveComponent();


    bool operator==(const Entity& other) const
    {
        return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
    }

    bool operator !=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    entt::entity m_Handle{entt::null};
    Scene* m_Scene = nullptr;
};

template <typename T, typename ... Args>
T& Entity::AddComponent(Args&&... args)
{
    T& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
    return component;
}

template <typename T, typename ... Args>
T& Entity::AddOrReplaceComponent(Args&&... args)
{
    T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
    return component;
}

template <typename T>
T& Entity::GetComponent()
{
    return m_Scene->m_Registry.get<T>(m_Handle);
}

template <typename T>
bool Entity::HasComponent()
{
    return m_Scene->m_Registry.has<T>(m_Handle);
}

template <typename T>
void Entity::RemoveComponent()
{
    m_Scene->m_Registry.remove<T>(m_Handle);
}
