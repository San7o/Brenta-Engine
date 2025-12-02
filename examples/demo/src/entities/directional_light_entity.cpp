// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/directional_light_component.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/directional_light_entity.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace brenta;

void init_directional_light_entity()
{
  /* Create the light entity */
  auto light_entity = world::new_entity();

  /* Add the light component */
  auto light_component = DirectionalLightComponent(
    glm::vec3(0.2f, -1.0f, -0.3f), // direction
    glm::vec3(0.7f, 0.7f, 0.7f),   // ambient
    glm::vec3(0.5f, 0.5f, 0.5f),   // diffuse
    glm::vec3(1.0f, 1.0f, 1.0f),   // specular
    1.0f,                          // intensity
    std::vector<brenta::types::shader_name_t>{"default_shader"});
  world::add_component<DirectionalLightComponent>(light_entity,
                                                  std::move(light_component));
}
