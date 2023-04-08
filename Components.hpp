#pragma once
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct TransformComponent
{
    glm::vec3 Position{0,0,0};
    glm::vec3 Rotation{0, 0, 0};
    glm::vec3 Scale{50, 50, 1};

    glm::mat4 GetTransform() const
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Position)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct  ColorComponent
{
    glm::vec3 Color{1.0, 0.0, 0.0};

    explicit ColorComponent(const glm::vec3& color)
        : Color(color)
    {
    }
};
class b2Body;
struct Rigidbody2DComponent
{
    enum class BodyType { Static = 0, Dynamic, Kinematic };
    BodyType Type = BodyType::Static;
    bool FixedRotation = false;

    // Storage for runtime
    b2Body* RuntimeBody = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
    glm::vec2 Offset = { 0.0f, 0.0f };
    glm::vec2 Size = { 0.5f, 0.5f };

    // TODO(Yan): move into physics material in the future maybe
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    float RestitutionThreshold = 0.5f;

    // Storage for runtime
    void* RuntimeFixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};