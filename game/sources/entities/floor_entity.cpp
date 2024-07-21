#include "entities/floor_entity.h"

#include "engine.h"
#include "components/model_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitFloorEntity() {

    /* Create the player entity */
    auto floor_entity = World::NewEntity();

    if (Shader::GetId("default_shader") == (unsigned int) -1) {
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
