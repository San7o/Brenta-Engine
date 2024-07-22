#include "entities/cube_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/transform_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitCubeEntity() {

    /* Create the cube entity */
    auto cube_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent(cube_entity, "TransformComponent", transform_component);


    /* Load the shader */
    if (Shader::GetId("cube_shader") == 0) {
        Shader::NewShader("cube_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/simple_cube/simple_cube.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, 32.0f, "cube_shader");
    World::AddComponent(cube_entity, "ModelComponent", model_component);
}
