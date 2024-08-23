#include "entities/floor_entity.hpp"

#include "engine.hpp"
#include "ecs.hpp"
#include "components/model_component.hpp"
#include "components/transform_component.hpp"

#include <filesystem>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;
using namespace Brenta;

void InitFloorEntity() {

    /* Create the floor entity */
    auto floor_entity = World::NewEntity();


    /* Add the transform component */
    auto transform_component = TransformComponent(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f),
            1.0f
    );
    World::AddComponent<TransformComponent>(floor_entity, transform_component);


    if (Shader::GetId("default_shader") == 0) {
        /* Load the shader */
        Shader::New(
            "default_shader",
            GL_VERTEX_SHADER,
            std::filesystem::absolute("game/shaders/shader.vs"),
            GL_FRAGMENT_SHADER,
            std::filesystem::absolute("game/shaders/shader.fs")
        );
    }

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/pane/pane.obj"));

    /* Add the model component */
    auto model_component = ModelComponent(model, 32.0f, "default_shader");
    World::AddComponent<ModelComponent>(floor_entity, std::move(model_component));
}
