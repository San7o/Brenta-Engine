#include "entities/directional_light_entity.h"

#include "engine.h"
#include "ecs.h"
#include "components/directional_light_component.h"
#include "components/transform_component.h"
#include "components/model_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <filesystem>

using namespace ECS;

void InitDirectionalLightEntity() {

    /* Create the light entity */
    auto light_entity = World::NewEntity();

    /* Add the light component */
    auto light_component = DirectionalLightComponent(
        glm::vec3(0.2f, -1.0f, -0.3f), // direction
        glm::vec3(0.7f, 0.7f, 0.7f),    // ambient
        glm::vec3(0.5f, 0.5f, 0.5f),    // diffuse
        glm::vec3(1.0f, 1.0f, 1.0f),    // specular
        1.0f,                           // intensity
        std::vector<Types::ShaderName>{"default_shader"}
    );
    World::AddComponent<DirectionalLightComponent>(light_entity, light_component);
}
