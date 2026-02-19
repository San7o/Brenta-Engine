// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/logger.hpp>
#include <cmath>

using namespace brenta;

Camera::Camera(Config conf)
{
  this->proj_type = conf.proj_type;
  this->fov = conf.fov;
  this->world_up = conf.world_up;
  this->front = conf.front;
  this->up = conf.up;
  this->right = conf.right;

  this->set_pos(conf.pos);   // updates world_pos and other members
  return;
}

glm::mat4 Camera::get_view_matrix() const
{
  try
  {
    Spherical spos = std::get<Spherical>(this->pos);
    return glm::lookAt(this->transform.get_pos(), spos.center, this->world_up);

  }
  catch ([[maybe_unused]] const std::bad_variant_access& ex)
  {
    return glm::lookAt(this->transform.get_pos(),
                       this->transform.get_pos() + this->front, this->up);
  }
}

// The projection matrix maps the verticies inside a cube called the
// Canonical Cube where all axis are between -1 and 1
glm::mat4 Camera::get_projection_matrix(int window_width,
                                        int window_height) const
{
  switch (this->proj_type)
  {
  case ProjectionType::Perspective:

    // The perspective calculates the position of a vertex from 3D
    // view space to clip space using a technique base on triangle
    // similarity.
    return glm::perspective(glm::radians(this->fov),
                            (float) window_width
                            / (float) window_height,
                            0.1f, 1000.0f);
  case ProjectionType::Orthographic:
    return glm::ortho((float) -window_width / 2.0f,
                      (float) window_width / 2.0f,
                      (float) -window_height / 2.0f,
                      (float) window_height / 2.0f, 0.1f, 100.0f);
  default:
    return glm::mat4(1.0f);
  }
}

void Camera::update_spherical(Spherical pos)
{
  this->transform.set_x(sin(pos.theta) * cos(pos.phi) * pos.radius + pos.center.x);
  this->transform.set_y(cos(pos.theta) * pos.radius + pos.center.y);
  this->transform.set_z(sin(pos.theta) * sin(pos.phi) * pos.radius + pos.center.z);
  
  return;
}

void Camera::update_aircraft(Aircraft pos)
{
  // calculate the new Front vector
  glm::vec3 new_front;
  new_front.x =
    cos(glm::radians(pos.yaw)) * cos(glm::radians(pos.pitch));
  new_front.y = sin(glm::radians(pos.pitch));
  new_front.z =
    sin(glm::radians(pos.yaw)) * cos(glm::radians(pos.pitch));
  this->front = glm::normalize(new_front);
  // also re-calculate the Right and Up vector
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up    = glm::normalize(glm::cross(this->right, this->front));
  this->transform.set_pos(pos.pos);
  return;
}

//
// Getters
//

Camera::ProjectionType Camera::get_projection_type() const
{
  return this->proj_type;
}

std::variant<Camera::Spherical, Camera::Aircraft> Camera::get_pos() const
{
  return this->pos;
}

Transform Camera::get_transform()
{
  return this->transform;
}

float Camera::get_fov() const
{
  return this->fov;
}

glm::vec3 Camera::get_front() const
{
  return this->front;
}

glm::vec3 Camera::get_up() const
{
  return this->up;
}

glm::vec3 Camera::get_right() const
{
  return this->right;
}

//
// Setters
//

void Camera::set_projection_type(ProjectionType projection_type)
{
  this->proj_type = projection_type;
}

void Camera::set_pos(std::variant<Spherical, Aircraft> new_pos)
{
  this->pos = new_pos;

  try
  {
    this->update_spherical(std::get<Spherical>(this->pos));
  }
  catch ([[maybe_unused]] const std::bad_variant_access& ex)
  {
    this->update_aircraft(std::get<Aircraft>(this->pos));
  }
  
  return;
}

glm::vec3 Camera::get_world_up() const
{
  return this->world_up;
}

void Camera::set_world_up(glm::vec3 world_up)
{
  this->world_up = world_up;
}

void Camera::set_fov(float fov)
{
  this->fov = fov;
}

void Camera::set_front(glm::vec3 front)
{
  this->front = front;
}

void Camera::set_up(glm::vec3 up)
{
  this->up = up;
}

void Camera::set_right(glm::vec3 right)
{
  this->right = right;
}

//
// Builder
//

// Camera::Spherical builder

Camera::Spherical::Builder &
Camera::Spherical::Builder::center(glm::vec3 center)
{
  this->scam.center = center;
  return *this;
}

Camera::Spherical::Builder &
Camera::Spherical::Builder::theta(float theta)
{
  this->scam.theta = theta;
  return *this;
}

Camera::Spherical::Builder &
Camera::Spherical::Builder::phi(float phi)
{
  this->scam.phi = phi;
  return *this;
}

Camera::Spherical::Builder &
Camera::Spherical::Builder::radius(float radius)
{
  this->scam.radius = radius;
  return *this;
}

Camera::Spherical Camera::Spherical::Builder::build()
{
  return this->scam;
}

// Camera::aircraft Builder

Camera::Aircraft::Builder &
Camera::Aircraft::Builder::pos(glm::vec3 pos)
{
  this->acam.pos = pos;
  return *this;
}

Camera::Aircraft::Builder &
Camera::Aircraft::Builder::yaw(float yaw)
{
  this->acam.yaw = yaw;
  return *this;
}

Camera::Aircraft::Builder &
Camera::Aircraft::Builder::pitch(float pitch)
{
  this->acam.pitch = pitch;
  return *this;
}

Camera::Aircraft::Builder &
Camera::Aircraft::Builder::roll(float roll)
{
  this->acam.roll = roll;
  return *this;
}

Camera::Aircraft Camera::Aircraft::Builder::build()
{
  return this->acam;
}

// Camera Builder

Camera::Builder &
Camera::Builder::projection_type(Camera::ProjectionType projection_type)
{
  this->conf.proj_type = projection_type;
  return *this;
}

Camera::Builder &Camera::Builder::position(std::variant<Camera::Spherical,
                                           Camera::Aircraft> pos)
{
  this->conf.pos = pos;
  return *this;
}

Camera::Builder &Camera::Builder::fov(float fov)
{
  this->conf.fov = fov;
  return *this;
}

Camera::Builder &Camera::Builder::world_up(glm::vec3 world_up)
{
  this->conf.world_up = world_up;
  return *this;
}

Camera::Builder &Camera::Builder::front(glm::vec3 front)
{
  this->conf.front = front;
  return *this;
}

Camera::Builder &Camera::Builder::up(glm::vec3 up)
{
  this->conf.up = up;
  return *this;
}

Camera::Builder &Camera::Builder::right(glm::vec3 right)
{
  this->conf.right = right;
  return *this;
}

brenta::Camera Camera::Builder::build()
{
  return Camera(this->conf);
}
