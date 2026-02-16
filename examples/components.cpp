// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// components.cpp
// ==============
//
// Examples of various components in the ECS system.
//

#include <brenta/renderer/opengl/shader.hpp>
#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace viotecs;

struct PhysicsComponent : Component
{
  float     mass;
  float     density;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  bool      isElastic;

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

struct PlayerComponent : Component
{
  PlayerComponent()
  {
  }
};

struct TransformComponent : Component
{
  glm::vec3 position;
  glm::vec3 rotation;
  float     scale;

  TransformComponent()
      : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
  {
  }
  TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
      : position(position), rotation(rotation), scale(scale)
  {
  }
};

struct PointLightComponent : Component
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  // Attenuation
  float constant; // at least 1
  float linear;
  float quadratic;
  float strength;
  std::vector<brenta::Shader::Name> shaders;

  PointLightComponent()
      : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)),
        specular(glm::vec3(0.0f)), constant(1.0f), linear(0.0f),
        quadratic(0.0f), strength(0.0f)
  {
  }
  PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                      float constant, float linear, float quadratic,
                      float strength,
                      std::vector<brenta::Shader::Name> shaders)
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
