#include "../Scene.hpp"

#include <spdlog/spdlog.h>

#include "../Components.hpp"
#include "../Entity.hpp"

Entity Scene::CreateEntity()
{
    Entity entity = {
        m_Registry.create(), this
    };

    entity.AddComponent<TransformComponent>();

    this->m_Entities.push_back(entity);

    return entity;
}

void Scene::DestroyEntity(Entity entity)
{
    m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
}


namespace Utils
{
    inline b2BodyType toBox2DType(Rigidbody2DComponent::BodyType type)
    {
        switch (type)
        {
        case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }
        return b2_staticBody;
    }
}


void Scene::CreatePhysicsEntity(Entity entity)
{
    auto& transform = entity.GetComponent<TransformComponent>();
    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

    b2BodyDef bodyDef;
    bodyDef.type = Utils::toBox2DType(rb2d.Type);
    bodyDef.position.Set(transform.Position.x, transform.Position.y);
    bodyDef.angle = transform.Rotation.z;

    b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
    body->SetFixedRotation(rb2d.FixedRotation);
    rb2d.RuntimeBody = body;


    //mass
    b2MassData massData;
    massData.mass = 0.5f;
    body->SetMassData(&massData);

  

    if (entity.HasComponent<BoxCollider2DComponent>())
    {
        auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

        b2PolygonShape boxShape;
        boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = bc2d.Density;
        fixtureDef.friction = bc2d.Friction;
        fixtureDef.restitution = bc2d.Restitution;
        fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
        body->CreateFixture(&fixtureDef);

    }
}

void Scene::OnStart()
{
    m_PhysicsWorld = new b2World(b2Vec2{0.0f, 9.8f});
    auto view = m_Registry.view<Rigidbody2DComponent>();
    for (auto e : view)
    {
        Entity entity = {e, this};
        CreatePhysicsEntity(entity);
    }
}

void Scene::OnUpdate(float ts)
{
    const int32_t velocityIterations = 6;
    const int32_t positionIterations = 2;
    m_PhysicsWorld->Step(1.0f / 255.0f, velocityIterations, positionIterations);

    auto view = m_Registry.view<Rigidbody2DComponent>();

    for (auto e : view)
    {
        Entity entity = {e, this};
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();


        b2Body* body = (b2Body*)rb2d.RuntimeBody;

        const auto& position = body->GetPosition();
        transform.Position.x = position.x;
        transform.Position.y = position.y;
        transform.Rotation.z = body->GetAngle();
    }
}

void Scene::OnStop()
{
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
}
