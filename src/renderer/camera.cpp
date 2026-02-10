// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/logger.hpp>
#include <cmath>

using namespace brenta;

camera::camera(config conf)
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

glm::mat4 camera::get_view_matrix() const
{
  try
  {
    spherical spos = std::get<spherical>(this->pos);
    return glm::lookAt(this->world_pos, spos.center, this->world_up);

  }
  catch ([[maybe_unused]] const std::bad_variant_access& ex)
  {
    return glm::lookAt(this->world_pos, this->world_pos + this->front, this->up);
  }
}

glm::mat4 camera::get_projection_matrix(int window_width,
                                        int window_height) const
{
  switch (this->proj_type)
  {
  case projection_type::perspective:
    return glm::perspective(glm::radians(this->fov),
                            (float) window_width
                            / (float) window_height,
                            0.1f, 1000.0f);
  case projection_type::orthographic:
    return glm::ortho((float) -window_width / 2.0f,
                      (float) window_width / 2.0f,
                      (float) -window_height / 2.0f,
                      (float) window_height / 2.0f, 0.1f, 100.0f);
  default:
    return glm::mat4(1.0f);
  }
}

void camera::update_spherical(spherical pos)
{
  this->world_pos.x = sin(pos.theta) * cos(pos.phi) * pos.radius
    + pos.center.x;
  this->world_pos.y = cos(pos.theta) * pos.radius + pos.center.y;
  this->world_pos.z = sin(pos.theta) * sin(pos.phi) * pos.radius
    + pos.center.z;
}

void camera::update_aircraft(aircraft pos)
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
  this->up = glm::normalize(glm::cross(this->right, this->front));
}

//
// Getters
//

camera::projection_type camera::get_projection_type() const
{
  return this->proj_type;
}

std::variant<camera::spherical, camera::aircraft> camera::get_pos() const
{
  return this->pos;
}

glm::vec3 camera::get_world_pos() const
{
  return this->world_pos;
}

float camera::get_fov() const
{
  return this->fov;
}

glm::vec3 camera::get_front() const
{
  return this->front;
}

glm::vec3 camera::get_up() const
{
  return this->up;
}

glm::vec3 camera::get_right() const
{
  return this->right;
}

//
// Setters
//

void camera::set_projection_type(projection_type projection_type)
{
  this->proj_type = projection_type;
}

void camera::set_pos(std::variant<spherical, aircraft> new_pos)
{
  this->pos = new_pos;

  try
  {
    this->update_spherical(std::get<spherical>(this->pos));
  }
  catch ([[maybe_unused]] const std::bad_variant_access& ex)
  {
    this->update_aircraft(std::get<aircraft>(this->pos));
  }
  
  return;
}

glm::vec3 camera::get_world_up() const
{
  return this->world_up;
}

void camera::set_world_up(glm::vec3 world_up)
{
  this->world_up = world_up;
}

void camera::set_fov(float fov)
{
  this->fov = fov;
}

void camera::set_front(glm::vec3 front)
{
  this->front = front;
}

void camera::set_up(glm::vec3 up)
{
  this->up = up;
}

void camera::set_right(glm::vec3 right)
{
  this->right = right;
}

//
// Builder
//

// camera::spherical builder

camera::spherical::builder &
camera::spherical::builder::center(glm::vec3 center)
{
  this->scam.center = center;
  return *this;
}

camera::spherical::builder &
camera::spherical::builder::theta(float theta)
{
  this->scam.theta = theta;
  return *this;
}

camera::spherical::builder &
camera::spherical::builder::phi(float phi)
{
  this->scam.phi = phi;
  return *this;
}

camera::spherical::builder &
camera::spherical::builder::radius(float radius)
{
  this->scam.radius = radius;
  return *this;
}

camera::spherical camera::spherical::builder::build()
{
  return this->scam;
}

// camera::aircraft builder

camera::aircraft::builder &
camera::aircraft::builder::pos(glm::vec3 pos)
{
  this->acam.pos = pos;
  return *this;
}

camera::aircraft::builder &
camera::aircraft::builder::yaw(float yaw)
{
  this->acam.yaw = yaw;
  return *this;
}

camera::aircraft::builder &
camera::aircraft::builder::pitch(float pitch)
{
  this->acam.pitch = pitch;
  return *this;
}

camera::aircraft::builder &
camera::aircraft::builder::roll(float roll)
{
  this->acam.roll = roll;
  return *this;
}

camera::aircraft camera::aircraft::builder::build()
{
  return this->acam;
}

// Camera builder

camera::builder &
camera::builder::projection_type(camera::projection_type projection_type)
{
  this->conf.proj_type = projection_type;
  return *this;
}

camera::builder &camera::builder::position(std::variant<camera::spherical,
                                           camera::aircraft> pos)
{
  this->conf.pos = pos;
  return *this;
}

camera::builder &camera::builder::fov(float fov)
{
  this->conf.fov = fov;
  return *this;
}

camera::builder &camera::builder::world_up(glm::vec3 world_up)
{
  this->conf.world_up = world_up;
  return *this;
}

camera::builder &camera::builder::front(glm::vec3 front)
{
  this->conf.front = front;
  return *this;
}

camera::builder &camera::builder::up(glm::vec3 up)
{
  this->conf.up = up;
  return *this;
}

camera::builder &camera::builder::right(glm::vec3 right)
{
  this->conf.right = right;
  return *this;
}

brenta::camera camera::camera::builder::build()
{
  return camera(this->conf);
}
