#include "entities/robot_entity.hpp"

#include "engine.hpp"
#include "ecs.hpp"
#include "components/model_component.hpp"
#include "components/transform_component.hpp"

#include <filesystem>

using namespace ECS;
using namespace ECS::Types;

void InitRobotEntity() {

    /* Create the cube entity */
    auto cube_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = TransformComponent(
            glm::vec3(0.0f, 5.0f, 0.0f),
            glm::vec3(0.0f, -90.0f, 0.0f),
            5.0f
    );
    World::AddComponent<TransformComponent>(cube_entity, transform_component);


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
    Model model(std::filesystem::absolute("assets/models/robot_sprite/robot_sprite.obj"), GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_TRUE, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST, false);

    /* Add the model component */
    auto model_component = ModelComponent(
                    model,
                    32.0f,
                    "default_shader",
                    true,
                    4,
                    0
    );

    World::AddComponent<ModelComponent>(cube_entity, model_component);
}
