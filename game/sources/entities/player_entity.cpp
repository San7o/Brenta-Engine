#include "entities/player_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/player_component.h"
#include "components/transform_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitPlayerEntity() {

    /* Create the player entity */
    auto player_entity = World::NewEntity();


    /* Add the player component */
    auto player_component = std::make_shared<PlayerComponent>();
    World::AddComponent(player_entity, "PlayerComponent", player_component);


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 1.8f, -5.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(player_entity, "TransformComponent", transform_component);


    /* Load the shader */
    if (Shader::GetId("default_shader") == 0) {
        Shader::NewShader("default_shader",
                          std::filesystem::absolute("game/shaders/shader.vs"),
                          std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/backpack/backpack.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, 32.0f, "default_shader");
    World::AddComponent(player_entity, "ModelComponent", model_component);
}
