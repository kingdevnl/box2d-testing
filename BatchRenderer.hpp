#pragma once
#include "Components.hpp"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
};

class BatchRenderer
{

private:
    std::vector<Vertex> m_Verts;

public:
    void AddQuad(TransformComponent* transform, glm::vec3 color)
    {
        glm::mat4 mat = transform->GetTransform();


        auto add = [this] (glm::vec4 p, glm::vec3 c)
        {
            this->m_Verts.push_back({
                {p.x, p.y, p.z}, c
            });
        };

        add(mat * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color);
        add(mat * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), color);
        add(mat * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), color);

        add(mat * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), color);
        add(mat * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), color);
        add(mat * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color);
    }
    void Draw()
    {
        glBegin(GL_TRIANGLES);
        for(auto& vert : this->m_Verts)
        {
            auto pos = vert.pos;
            auto color = vert.color;
            glColor3f(color.r, color.g,color.b);
            glVertex3f(pos.x, pos.y, pos.z);
        }
        glEnd();
    }
	
    void NextBatch()
    {
        this->m_Verts.clear();
    }
};