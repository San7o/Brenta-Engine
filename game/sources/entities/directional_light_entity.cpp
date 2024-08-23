#include "entities/directional_light_entity.hpp"

#include "components/directional_light_component.hpp"
#include "components/model_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Brenta::ECS;
using namespace Brenta;

void InitDirectionalLightEntity ()
{
    /* Create the light entity */
    auto light_entity = World::NewEntity ();

    /* Add the light component */
    auto light_component = DirectionalLightComponent (
        glm::vec3 (0.2f, -1.0f, -0.3f), // direction
        glm::vec3 (0.7f, 0.7f, 0.7f),   // ambient
        glm::vec3 (0.5f, 0.5f, 0.5f),   // diffuse
        glm::vec3 (1.0f, 1.0f, 1.0f),   // specular
        1.0f,                           // intensity
        std::vector<Brenta::Types::ShaderName>{"default_shader"});
    World::AddComponent<DirectionalLightComponent> (
        light_entity, std::move (light_component));
}
