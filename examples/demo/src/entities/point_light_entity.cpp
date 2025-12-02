// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/point_light_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/entities/point_light_entity.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace brenta;

void init_point_light_entity()
{
  /* Create the light entity */
  auto light_entity = world::new_entity();

  /* Add the light component */
  auto light_component = PointLightComponent(
    glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.0032f, 1.0f,
    std::vector<brenta::types::shader_name_t>{"default_shader"});
  world::add_component<PointLightComponent>(light_entity, light_component);

  /* Add a mesh */
  /*
  if (Shader::GetId("default_shader") == 0) {
      Shader::NewShader("default_shader",
                        std::filesystem::absolute("exmaples/demo/shaders/shader.vs"),
                        std::filesystem::absolute("examples/demo/shaders/shader.fs"));
  }
  Model
  model(std::filesystem::absolute("examples/assets/models/simple_cube/simple_cube.obj"));
  auto model_component = ModelComponent(model, 32.0f, "default_shader");
  World::AddComponent<ModelComponent>(light_entity, model_component);
  */

  /* Add the transform component */
  auto transform_component =
    TransformComponent(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f), 0.5f);
  world::add_component<TransformComponent>(light_entity, transform_component);
}
