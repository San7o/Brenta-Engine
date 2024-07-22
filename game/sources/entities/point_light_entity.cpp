#include "entities/point_light_entity.h"

#include "engine.h"
#include "components/point_light_component.h"
#include "components/transform_component.h"
#include "components/model_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <filesystem>

using namespace ECS;

void InitPointLightEntity() {

    /* Create the light entity */
    auto light_entity = World::NewEntity();


    /* Add the light component */
    auto light_component = std::make_shared<PointLightComponent>(
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            1.0f,
            0.09f,
            0.0032f,
            3.0f,
            std::vector<Types::ShaderName>{"default_shader"}
    );
    World::AddComponent(light_entity, "PointLightComponent", light_component);


    
    /* Add a mesh */
    /*
    if (Shader::GetId("default_shader") == 0) {
        Shader::NewShader("default_shader",
                          std::filesystem::absolute("game/shaders/shader.vs"),
                          std::filesystem::absolute("game/shaders/shader.fs"));
    }
    Model model(std::filesystem::absolute("assets/models/simple_cube/simple_cube.obj"));
    auto model_component = std::make_shared<ModelComponent>(model, "default_shader");
    World::AddComponent(light_entity, "ModelComponent", model_component);
    */


    /* Add the transform component */
    auto transform_component = std::make_shared<TransformComponent>(
            glm::vec3(0.0f, 4.0f, 3.0f),
            glm::vec3(0.0f),
            0.5f
    );
    World::AddComponent(light_entity, "TransformComponent", transform_component);

}
