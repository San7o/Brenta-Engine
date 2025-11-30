// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Examples of various components that can be used in an ECS system.
 */

#include <brenta/engine.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;
using namespace viotecs;

struct PhysicsComponent : component
{
  float mass;
  float density;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  bool isElastic;

  PhysicsComponent()
      : mass(0.0f), density(0.0f), velocity(glm::vec3(0.0f)),
        acceleration(glm::vec3(0.0f)), isElastic(false)
  {
  }
  PhysicsComponent(float mass, float density, glm::vec3 velocity,
                   glm::vec3 acceleration, bool isElastic)
      : mass(mass), density(density), velocity(velocity),
        acceleration(acceleration), isElastic(isElastic)
  {
  }
};

struct PlayerComponent : component
{
  PlayerComponent()
  {
  }
};

struct TransformComponent : component
{
  glm::vec3 position;
  glm::vec3 rotation;
  float scale;

  TransformComponent()
      : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
  {
  }
  TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
      : position(position), rotation(rotation), scale(scale)
  {
  }
};

struct PointLightComponent : component
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  // Attenuation
  float constant; // at least 1
  float linear;
  float quadratic;
  float strength;
  std::vector<brenta::types::shader_name_t> shaders;

  PointLightComponent()
      : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)),
        specular(glm::vec3(0.0f)), constant(1.0f), linear(0.0f),
        quadratic(0.0f), strength(0.0f)
  {
  }
  PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                      float constant, float linear, float quadratic,
                      float strength,
                      std::vector<brenta::types::shader_name_t> shaders)
      : ambient(ambient), diffuse(diffuse), specular(specular),
        constant(constant), linear(linear), quadratic(quadratic),
        strength(strength), shaders(shaders)
  {
  }
};

int main()
{
  return 0;
}
