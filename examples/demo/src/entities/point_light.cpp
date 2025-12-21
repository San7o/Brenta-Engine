// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model.hpp>
#include <demo/components/point_light.hpp>
#include <demo/components/transform.hpp>
#include <demo/entities/point_light.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace viotecs;
using namespace brenta;

void init_point_light_entity()
{
  auto light = world::new_entity()
    .add_component<PointLightComponent>(glm::vec3(0.1f, 0.1f, 0.1f),
                                        glm::vec3(0.5f, 0.5f, 0.5f),
                                        glm::vec3(1.0f, 1.0f, 1.0f),
                                        1.0f, 0.09f, 0.0032f, 1.0f,
                                        std::vector<brenta::shader::name_t>{"default_shader"})
    .add_component<TransformComponent>(glm::vec3(0.0f, 10.0f, 0.0f),
                                       glm::vec3(0.0f), 0.5f);
}
