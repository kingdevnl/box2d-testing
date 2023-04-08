#include "Init.hpp"
#include "BatchRenderer.hpp"
#include "Entity.hpp"
#include "entt.hpp"

Init init;
BatchRenderer renderer;


Scene scene;

glm::vec3 randomColor()
{
    float r = rand() % 255;
    float g = rand() % 255;
    float b = rand() % 255;

    return glm::vec3(r, g, b) / glm::vec3(255);
}


void Initialize()
{
    spdlog::info("Hello, World");


    // for(int i = 0; i < 20; i++)
    // {
    // 	auto entity = scene.CreateEntity();
    // 	auto& transform = entity.GetComponent<TransformComponent>();
    // 	transform.Position.x =10+ rand() % (int)init.window_size.x-10;
    // 	transform.Position.y =10+ rand() % (int)init.window_size.y-10;
    // transform.Rotation.z = glm::radians(40.0f);
    // 	entity.AddComponent<ColorComponent>(randomColor());
    // }

    {
        auto entity = scene.CreateEntity();
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.Position.x = 700;
        transform.Position.y = 200;
        transform.Scale = glm::vec3(300, 40, 1.0);
        transform.Rotation.z = glm::radians(340.0f);
        entity.AddComponent<ColorComponent>(glm::vec3(1, 0, 0));
        auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
        rb2d.Type = Rigidbody2DComponent::BodyType::Static;
        entity.AddComponent<BoxCollider2DComponent>();
    }

    {
        auto entity = scene.CreateEntity();
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.Position.x = 500;
        transform.Position.y = 600;
        transform.Scale = glm::vec3(500, 40, 1.0);
        transform.Rotation.z = glm::radians(-340.0f);
        entity.AddComponent<ColorComponent>(glm::vec3(1, 0, 0));
        auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
        rb2d.Type = Rigidbody2DComponent::BodyType::Static;
        entity.AddComponent<BoxCollider2DComponent>();
    }
    //
    //
    // {
    // 	auto entity = scene.CreateEntity();
    // 	auto& transform = entity.GetComponent<TransformComponent>();
    // 	transform.Position.x = 500;
    // 	transform.Position.y = 200;
    // 	transform.Scale = glm::vec3(40, 40, 1.0);
    // 	transform.Rotation.z = 46.0f;
    // 	entity.AddComponent<ColorComponent>(glm::vec3(0, 0, 1));
    // 	auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
    // 	rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
    // 	auto& bc2d =  entity.AddComponent<BoxCollider2DComponent>();
    // 	// bc2d.Restitution = 0.2;
    // }

    scene.OnStart();
}


void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (button == GLFW_MOUSE_BUTTON_1)
    {
        spdlog::info("Click at {} {}", x, y);

        auto entity = scene.CreateEntity();
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.Position.x = x;
        transform.Position.y = y;
        transform.Scale = glm::vec3(40, 40, 1.0);
        entity.AddComponent<ColorComponent>(glm::vec3(0, 1, 0));
        auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
        rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
        auto& bc2d = entity.AddComponent<BoxCollider2DComponent>();
        bc2d.Restitution = 0.5;
        scene.CreatePhysicsEntity(entity);
    }
    if (button == GLFW_MOUSE_BUTTON_2)
    {
        spdlog::info("RightClick at {} {}", x, y);

        auto entity = scene.CreateEntity();
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.Position.x = x;
        transform.Position.y = y;
        transform.Scale = glm::vec3(40, 40, 1.0);
        entity.AddComponent<ColorComponent>(glm::vec3(0, 1, 1));
        auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
        rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
        auto& bc2d = entity.AddComponent<BoxCollider2DComponent>();
        bc2d.Restitution = 0.5;
        scene.CreatePhysicsEntity(entity);
        //apply rotation force so it spins like a motor infinitely
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_SPACE)
    {
        spdlog::info("Space pressed");
        auto view = scene.GetAllEntitiesWith<TransformComponent, Rigidbody2DComponent>();
        for (auto e : view)
        {
            
            Entity entity = {e, &scene};
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
            if(rb2d.Type != Rigidbody2DComponent::BodyType::Dynamic)
                continue;

            //shoot object in direction of mouse
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            glm::vec2 mousePos = {x, y};
            auto& transform = entity.GetComponent<TransformComponent>();
            glm::vec3 pos = transform.Position;
            glm::vec2 dir = mousePos - glm::vec2(pos.x, pos.y);
            dir = glm::normalize(dir);

            spdlog::info("DIR: {} {}", dir.x, dir.y);
            rb2d.RuntimeBody->ApplyLinearImpulse(b2Vec2(dir.x, dir.y), rb2d.RuntimeBody->GetWorldCenter(), true);
        }
    }
}

void OnUpdate(float ts)
{
    scene.OnUpdate(ts);

    auto& size = init.window_size;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, size.x, size.y, 0, 0, 1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetMouseButtonCallback(init.window, mouseCallback);
    glfwSetKeyCallback(init.window, keyCallback);


    renderer.NextBatch();

    auto view = scene.GetAllEntitiesWith<TransformComponent, ColorComponent>();

    for (auto entity : view)
    {
        auto [transform, color] = view.get<TransformComponent, ColorComponent>(entity);
        renderer.AddQuad(&transform, color.Color);
    }


    renderer.Draw();
}


int main()
{
    init.window_size = {1080, 720};
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    init.window = glfwCreateWindow(static_cast<int>(init.window_size.x), static_cast<int>(init.window_size.y),
                                   "Physics", 0, 0);
    glfwMakeContextCurrent(init.window);
    glfwSwapInterval(0);
    glewInit();

    Initialize();


    float lastFrameTime = 0;
    while (!glfwWindowShouldClose(init.window))
    {
        float time = (float)glfwGetTime();
        float ts = time - lastFrameTime;
        lastFrameTime = time;

        glfwPollEvents();
        OnUpdate(ts);
        glfwSwapBuffers(init.window);
    }
}
