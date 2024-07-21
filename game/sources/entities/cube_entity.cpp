#include "entities/cube_entity.h"

#include "engine.h"
#include "components/model_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitCubeEntity() {

    /* Create the player entity */
    auto cube_entity = World::NewEntity();

    /* Load the shader */
    if (Shader::GetId("cube_shader") == (unsigned int) -1) {
        Shader::NewShader("cube_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/simple_cube/simple_cube.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, "cube_shader");
    World::AddComponent(cube_entity, "ModelComponent", model_component);
}
