#include "entities/player_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/player_component.h"
#include "components/transform_component.h"
#include "components/sphere_collider_component.h"
#include "components/physics_component.h"
#include "components/particle_component.h"

#include <filesystem>
#include <memory>

using namespace ECS;
using namespace ECS::Types;

void InitPlayerEntity() {

    /* Create the player entity */
    auto player_entity = World::NewEntity();


    /* Add the player component */
    auto player_component = std::make_shared<PlayerComponent>(2.0f);
    World::AddComponent(player_entity, "PlayerComponent", player_component);


    /* Add the phydics component */
    auto physics_component1 = std::make_shared<PhysicsComponent>(
            1.0f,                         // mass
            1.0f,                         // density
            glm::vec3(0.0f, 0.0f, 0.0f),  // velocity
            glm::vec3(0.f, 0.0f, 0.0f),  // acceleration
            true                          // isElastic
    );
    World::AddComponent(player_entity, "PhysicsComponent", physics_component1);

    /* Add the sphere collider component */
    auto sphere_collider_component1 = std::make_shared<SphereColliderComponent>(1.0f);
    World::AddComponent(player_entity, "SphereColliderComponent", sphere_collider_component1);


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 0.0f, -3.0f),
            glm::vec3(0.0f, -90.0f, 0.0f),
            1.0f
    );
    World::AddComponent(player_entity, "TransformComponent", transform_component);


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
    Model model(std::filesystem::absolute(
                    "assets/gmtk2024/models/mosquito.obj"),
                    GL_REPEAT,
                    GL_NEAREST,
                    GL_NEAREST,
                    GL_TRUE,
                    GL_NEAREST_MIPMAP_LINEAR,
                    GL_NEAREST,
                    false
                );

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(
                    model, 32.0f, "default_shader", true, 16, 0, true,
                    2, 6);
    World::AddComponent(player_entity, "ModelComponent", model_component);


    /* Add the particle component */
    auto emitter_ptr = std::make_shared<ParticleEmitter>(
        glm::vec3(0.0f, 0.0f, -1.0f),     /* Starting position */
        glm::vec3(0.0f, 0.0f, 0.0f),     /* Starting velocity */
        glm::vec3(1.0f, 1.0f, 1.0f),     /* Starting spread */
        0.4f,                            /* Starting life */
        100,                             /* Starting particles */
        0.05f,                            /* Emission rate */
        0.015f,                            /* Scale */
        std::filesystem::absolute(
                "assets/gmtk2024/textures/trail.png"
        ).string(),                      /* Texture */
        1,                               /* Atlas width */
        1,                               /* Atlas height */
        0                                /* Atlas index */
    );
    auto particle_component = std::make_shared<ParticleComponent>(emitter_ptr);
    World::AddComponent(player_entity, "ParticleComponent", particle_component);
}
