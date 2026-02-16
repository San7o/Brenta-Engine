// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/shader.hpp>
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
  Transform transform = Transform()
    .translate(glm::vec3(0.0f, 10.0f, 0.0f))
    .scale(glm::vec3(0.5f));
  
  auto light = World::new_entity()
    .add_component<PointLightComponent>(glm::vec3(0.1f, 0.1f, 0.1f), // ambient
                                        glm::vec3(0.5f, 0.5f, 0.5f), // diffuse
                                        glm::vec3(1.0f, 1.0f, 1.0f), // specular
                                        1.0f,      // constant, > 1
                                        0.09f,     // linear
                                        0.00032f,  // quadratic
                                        1.0f,      // strength
                                        std::vector<Shader::Name>{"default_shader"})
    .add_component<TransformComponent>(transform);
}
