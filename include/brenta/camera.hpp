// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <variant>

namespace brenta
{

/**
 * @brief The Camera class
 *
 * This class represents a camera in the 3D world. It is used to
 * define the position, orientation and projection of the camera.  The
 * class provides a builder pattern to create a camera object.
 *
 * ## Camera type
 *
 * The camera can be of two types: aircraft or spherical. The aircraft
 * type is used to represent a camera that moves in the world space
 * directions (x, y, z), while the spherical type is used to represent
 * a camera that rotates around a center point (you essentially
 * control the angle and radius around the center). They use
 * respectively euler angles and spherical coordinates to represent
 * the position of the camera.
 *
 * ## Projection
 *
 * The camera can have two types of projection: perspective or
 * orthographic.
 */
class camera
{
public:

  class spherical;
  class aircraft;
  class position;
  enum class projection_type;
  enum class movement;
  
  class config;
  class builder;
  
  // Constructors

  camera() = default;
  camera(config conf);

  // Getters

  std::variant<spherical, aircraft> get_pos() const;
  glm::vec3        get_world_pos() const;
  float            get_fov() const;
  glm::mat4        get_view_matrix() const;
  projection_type  get_projection_type() const;
  glm::mat4        get_projection_matrix(int window_width,
                                         int window_height) const;
  glm::vec3        get_world_up() const;
  glm::vec3        get_front() const;
  glm::vec3        get_up() const;
  glm::vec3        get_right() const;

  // Setters

  void set_pos(std::variant<spherical, aircraft> pos);
  void set_projection_type(projection_type proj_type);
  void set_fov(float fov);
  void set_world_up(glm::vec3 world_up);
  void set_front(glm::vec3 front);
  void set_up(glm::vec3 up);
  void set_right(glm::vec3 right);

  class spherical
  {
  public:

    class builder;
    
    glm::vec3 center;
    float     theta;
    float     phi;
    float     radius;
  
    spherical() = default;
    spherical(glm::vec3 center, float theta,
              float phi, float radius)
      : center(center), theta(theta), phi(phi), radius(radius) {}
  };
  
  class aircraft
  {
  public:

    class builder;
    
    glm::vec3 pos;
    float     yaw;
    float     pitch;
    float     roll;

    aircraft() = default;
    aircraft(glm::vec3 pos)
      : pos(pos) {}
    aircraft(glm::vec3 pos, float yaw, float pitch, float roll)
      : pos(pos), yaw(yaw), pitch(pitch), roll(roll) {}
    aircraft(const builder &builder);
    
  };
  
private:

  std::variant<spherical, aircraft> pos;
  projection_type proj_type;
  float           fov;                 // field of view / zoom

  glm::vec3   front;
  glm::vec3   up;
  glm::vec3   right;
  glm::vec3   world_up;

  // Remember to always use set_pos when changing the position,
  // never modify this value manually
  glm::vec3   world_pos;

  void update_spherical(spherical pos);
  void update_aircraft(aircraft pos);
};

enum class camera::projection_type
{
  perspective,
  orthographic
};

enum class camera::movement
{
  forward,
  backward,
  left,
  right
};

class camera::spherical::builder
{
public:

  builder() = default;
  
  camera::spherical::builder &center(glm::vec3 center);
  camera::spherical::builder &theta(float theta);
  camera::spherical::builder &phi(float phi);
  camera::spherical::builder &radius(float radius);

  camera::spherical build();
  
private:
  camera::spherical scam;
};

class camera::aircraft::builder
{
public:

  builder() = default;

  camera::aircraft::builder& pos(glm::vec3 pos);
  camera::aircraft::builder& yaw(float yaw);
  camera::aircraft::builder& pitch(float pitch);
  camera::aircraft::builder& roll(float roll);

  camera::aircraft build();
  
private:
  camera::aircraft acam;
};
  
struct camera::config
{
  std::variant<spherical, aircraft> pos;
  projection_type proj_type;
  float           fov;
  glm::vec3       world_up;
  glm::vec3       front;
  glm::vec3       up;
  glm::vec3       right;
};
  
class camera::builder
{
private:

  camera::config conf = {
    camera::aircraft(glm::vec3(0.0)),
    camera::projection_type::perspective,
    45.0f,
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
  };

public:
  
  builder &projection_type(projection_type proj_type);
  builder &position(std::variant<spherical, aircraft> pos);
  builder &fov(float fov);
  builder &world_up(glm::vec3 worldUp);
  builder &front(glm::vec3 front);
  builder &up(glm::vec3 up);
  builder &right(glm::vec3 right);

  camera build();
};

} // namespace brenta
