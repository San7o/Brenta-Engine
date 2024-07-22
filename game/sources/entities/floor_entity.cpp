#include "entities/floor_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/transform_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitFloorEntity() {

    /* Create the floor entity */
    auto floor_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(floor_entity, "TransformComponent", transform_component);


    if (Shader::GetId("default_shader") == 0) {
        /* Load the shader */
        Shader::NewShader("default_shader",
                          std::filesystem::absolute("game/shaders/shader.vs"),
                          std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/pane/pane.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, "default_shader");
    World::AddComponent(floor_entity, "ModelComponent", model_component);
}
