// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/transform.hpp>

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
class Camera
{
public:

  class Spherical;
  class Aircraft;
  class Position;
  enum class ProjectionType;
  enum class Movement;
  
  class Config;
  class Builder;
  
  // Constructors

  Camera() = default;
  Camera(Config conf);

  // Getters

  std::variant<Spherical, Aircraft> get_pos() const;
  Transform        get_transform();
  float            get_fov() const;
  glm::mat4        get_view_matrix() const;
  ProjectionType   get_projection_type() const;
  glm::mat4        get_projection_matrix(int window_width,
                                         int window_height) const;
  glm::vec3        get_world_up() const;
  glm::vec3        get_front() const;
  glm::vec3        get_up() const;
  glm::vec3        get_right() const;

  // Setters

  void set_pos(std::variant<Spherical, Aircraft> pos);
  void set_projection_type(ProjectionType proj_type);
  void set_fov(float fov);
  void set_world_up(glm::vec3 world_up);
  void set_front(glm::vec3 front);
  void set_up(glm::vec3 up);
  void set_right(glm::vec3 right);

  class Spherical
  {
  public:

    class Builder;
    
    glm::vec3 center;
    float     theta;
    float     phi;
    float     radius;
  
    Spherical() = default;
    Spherical(glm::vec3 center, float theta,
              float phi, float radius)
      : center(center), theta(theta), phi(phi), radius(radius) {}
  };
  
  class Aircraft
  {
  public:

    class Builder;
    
    glm::vec3 pos;
    float     yaw;
    float     pitch;
    float     roll;

    Aircraft() = default;
    Aircraft(glm::vec3 pos)
      : pos(pos) {}
    Aircraft(glm::vec3 pos, float yaw, float pitch, float roll)
      : pos(pos), yaw(yaw), pitch(pitch), roll(roll) {}
    Aircraft(const Builder &builder);
    
  };
  
private:

  std::variant<Spherical, Aircraft> pos;
  ProjectionType  proj_type;
  float           fov;                 // field of view / zoom

  glm::vec3   front;
  glm::vec3   up;
  glm::vec3   right;
  glm::vec3   world_up;

  // Remember to always use set_pos when changing the position,
  // never modify this value manually
  Transform   transform;

  void update_spherical(Spherical pos);
  void update_aircraft(Aircraft pos);
};

enum class Camera::ProjectionType
{
  Perspective,
  Orthographic
};

enum class Camera::Movement
{
  Forward,
  Backward,
  Left,
  Right
};

class Camera::Spherical::Builder
{
public:

  Builder() = default;
  
  Builder &center(glm::vec3 center);
  Builder &theta(float theta);
  Builder &phi(float phi);
  Builder &radius(float radius);

  Camera::Spherical build();
  
private:
  Camera::Spherical scam;
};

class Camera::Aircraft::Builder
{
public:

  Builder() = default;

  Camera::Aircraft::Builder& pos(glm::vec3 pos);
  Camera::Aircraft::Builder& yaw(float yaw);
  Camera::Aircraft::Builder& pitch(float pitch);
  Camera::Aircraft::Builder& roll(float roll);

  Camera::Aircraft build();
  
private:
  Camera::Aircraft acam;
};
  
struct Camera::Config
{
  std::variant<Spherical, Aircraft> pos;
  ProjectionType  proj_type;
  float           fov;
  glm::vec3       world_up;
  glm::vec3       front;
  glm::vec3       up;
  glm::vec3       right;
};
  
class Camera::Builder
{
private:

  Camera::Config conf = {
    Camera::Aircraft(glm::vec3(0.0)),
    Camera::ProjectionType::Perspective,
    45.0f,
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
  };

public:
  
  Builder &projection_type(ProjectionType proj_type);
  Builder &position(std::variant<Spherical, Aircraft> pos);
  Builder &fov(float fov);
  Builder &world_up(glm::vec3 worldUp);
  Builder &front(glm::vec3 front);
  Builder &up(glm::vec3 up);
  Builder &right(glm::vec3 right);

  Camera build();
};

} // namespace brenta
