// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
#include <demo/components/directional_light.hpp>
#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/directional_light.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace viotecs;
using namespace brenta;

void init_directional_light_entity()
{
  auto light = world::new_entity()
    .add_component<DirectionalLightComponent>(glm::vec3(0.2f, -1.0f, -0.3f), // direction
                                              glm::vec3(0.7f, 0.7f, 0.7f),   // ambient
                                              glm::vec3(0.5f, 0.5f, 0.5f),   // diffuse
                                              glm::vec3(1.0f, 1.0f, 1.0f),   // specular
                                              1.0f,                          // intensity
                                              std::vector<brenta::shader::name_t>{"default_shader"});
}
