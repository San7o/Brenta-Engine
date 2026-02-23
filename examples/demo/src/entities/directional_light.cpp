// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <demo/components/directional_light.hpp>
#include <demo/components/model.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/directional_light.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "../../../../src/renderer/shaders/c/phong_vs.c"
#include "../../../../src/renderer/shaders/c/phong_fs.c"

using namespace viotecs;
using namespace brenta;

void init_directional_light_entity()
{
  auto shader = AssetManager::get_shader("default_shader");
  if (!shader)
  {
    shader = AssetManager::new_shader("default_shader", {
        { Shader::Type::Vertex,   phong_vs },
        { Shader::Type::Fragment, phong_fs } });
  }
  
  auto light = World::new_entity()
    .add_component<DirectionalLightComponent>(glm::vec3(0.2f, -1.0f, -0.3f), // direction
                                              glm::vec3(0.7f, 0.7f, 0.7f),   // ambient
                                              glm::vec3(0.5f, 0.5f, 0.5f),   // diffuse
                                              glm::vec3(1.0f, 1.0f, 1.0f),   // specular
                                              0.9f,                          // intensity
                                              shader);
}
