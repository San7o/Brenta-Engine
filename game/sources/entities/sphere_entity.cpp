#include "entities/sphere_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/transform_component.h"
#include "components/physics_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitSphereEntity() {

    /* Create the sphere entity */
    auto sphere_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(sphere_entity, "TransformComponent", transform_component);

    /* Add the physics component */
    auto physics_component = std::make_shared<PhysicsComponent>(
            1.0f,
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            true
    );
    World::AddComponent(sphere_entity, "PhysicsComponent", physics_component);


    /* Load the shader */
    if (Shader::GetId("default_shader") == 0) {
        Shader::NewShader("default_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/sphere/sphere.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, 32.0f, "default_shader");
    World::AddComponent(sphere_entity, "ModelComponent", model_component);
}
