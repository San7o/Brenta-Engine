#include "entities/robot_entity.h"

#include "engine.h"
#include "components/model_component.h"
#include "components/transform_component.h"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitRobotEntity() {

    /* Create the cube entity */
    auto cube_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 5.0f, 0.0f),
            glm::vec3(0.0f, -90.0f, 0.0f),
            5.0f
    );
    World::AddComponent(cube_entity, "TransformComponent", transform_component);


    /* Load the shader */
    if (Shader::GetId("default_shader") == 0) {
        Shader::NewShader("default_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/robot_sprite/robot_sprite.obj"), GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_TRUE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, false);

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(
                    model,
                    32.0f,
                    "default_shader",
                    true,
                    4,
                    0
    );

    World::AddComponent(cube_entity, "ModelComponent", model_component);
}
