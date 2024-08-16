#include "entities/sphere_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/transform_component.h"
#include "components/physics_component.h"
#include "components/sphere_collider_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitSphereEntity() {


    /* ************************************************
     *
    *    Sphere 1
    *
    * ************************************************ */

    /* Create the sphere entity */
    auto sphere_entity1 = World::NewEntity();


    /* Add the transform component */
    auto transform_component1 = std::make_shared<TransformComponent>(
            glm::vec3(2.0f, 1.0f, 0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(sphere_entity1, "TransformComponent", transform_component1);

    /* Add the physics component */
    auto physics_component1 = std::make_shared<PhysicsComponent>(
            1.0f,                         // mass
            1.0f,                         // density
            glm::vec3(0.0f, 0.0f, 0.0f),  // velocity
            glm::vec3(-1.f, 0.0f, 0.0f),  // acceleration
            true                          // isElastic
    );
    World::AddComponent(sphere_entity1, "PhysicsComponent", physics_component1);

    /* Add the sphere collider component */
    auto sphere_collider_component1 = std::make_shared<SphereColliderComponent>(1.0f);
    World::AddComponent(sphere_entity1, "SphereColliderComponent", sphere_collider_component1);

    /* Load the shader */
    if (Shader::GetId("default_shader") == 0) {
        Shader::New(
            "default_shader",
            GL_VERTEX_SHADER,
            std::filesystem::absolute("game/shaders/shader.vs"),
            GL_FRAGMENT_SHADER,
            std::filesystem::absolute("game/shaders/shader.fs")
        );
    }

    /* Load the model */
    Model model1(std::filesystem::absolute("assets/models/sphere/sphere.obj"));

    /* Add the model component */
    auto model_component1 = std::make_shared<ModelComponent>(model1, 32.0f, "default_shader");
    World::AddComponent(sphere_entity1, "ModelComponent", model_component1);



    /* ************************************************
     *
    *    Sphere 2
    *
    * ************************************************ */

    /* Create the sphere entity */
    auto sphere_entity2 = World::NewEntity();


    /* Add the transform component */
    auto transform_component2 = std::make_shared<TransformComponent>(
            glm::vec3(-2.0f, 1.0f, 0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(sphere_entity2, "TransformComponent", transform_component2);

    /* Add the physics component */
    auto physics_component2 = std::make_shared<PhysicsComponent>(
            1.0f,                         // mass
            1.0f,                         // density
            glm::vec3(0.0f, 0.0f, 0.0f),  // velocity
            glm::vec3(1.0f, 0.0f, 0.0f),  // acceleration
            true                          // isElastic
    );
    World::AddComponent(sphere_entity2, "PhysicsComponent", physics_component2);

    /* Add the sphere collider component */
    auto sphere_collider_component2 = std::make_shared<SphereColliderComponent>(1.0f);
    World::AddComponent(sphere_entity2, "SphereColliderComponent", sphere_collider_component2);

    /* Load the shader */
    if (Shader::GetId("default_shader") == 0) {
        Shader::New(
            "default_shader",
            GL_VERTEX_SHADER,
            std::filesystem::absolute("game/shaders/shader.vs"),
            GL_FRAGMENT_SHADER,
            std::filesystem::absolute("game/shaders/shader.fs")
        );
    }

    /* Load the model */
    Model model2(std::filesystem::absolute("assets/models/sphere/sphere.obj"));

    /* Add the model component */
    auto model_component2 = std::make_shared<ModelComponent>(model2, 32.0f, "default_shader");
    World::AddComponent(sphere_entity2, "ModelComponent", model_component2);
}
