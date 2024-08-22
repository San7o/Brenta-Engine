#include "entities/player_entity.hpp"

#include "engine.hpp"
#include "ecs.hpp"
#include "components/model_component.hpp"
#include "components/player_component.hpp"
#include "components/transform_component.hpp"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitPlayerEntity() {

    /* Create the player entity */
    auto player_entity = World::NewEntity();


    /* Add the player component */
    World::AddComponent<PlayerComponent>(player_entity, PlayerComponent());


    /* Add the transform component */
    auto transform_component = TransformComponent(
            glm::vec3(0.0f, 1.8f, -5.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent<TransformComponent>(player_entity, transform_component);


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
    Model model(std::filesystem::absolute("assets/models/backpack/backpack.obj"));

    /* Add the model component */
    auto model_component = ModelComponent(model, 32.0f, "default_shader");
    World::AddComponent<ModelComponent>(player_entity, model_component);
}
