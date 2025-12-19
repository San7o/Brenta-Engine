// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/camera.hpp>
#include <brenta/logger.hpp>
#include <cmath>

using namespace brenta;

camera::camera(camera_type cam_type,
               projection_type proj_type, glm::vec3 position,
               glm::vec3 world_up, glm::vec3 center, float movement_speed,
               float mouse_sensitivity, float zoom,
               types::spherical_coordinates spherical_coordinates,
               types::euler_angles euler_angles, glm::vec3 front, glm::vec3 up,
               glm::vec3 right)
{
  this->cam_type = cam_type;
  this->proj_type = proj_type;
  this->position = position;
  this->world_up = world_up;
  this->center = center;
  this->movement_speed = movement_speed;
  this->mouse_sensitivity = mouse_sensitivity;
  this->zoom = zoom;
  this->spherical_coordinates = spherical_coordinates;
  this->euler_angles = euler_angles;
  this->front = front;
  this->up = up;
  this->right = right;

  // Update the camera
  switch (this->cam_type)
  {
  case camera_type::spherical:
    spherical_to_cartesian();
    break;
  case camera_type::aircraft:
    update_camera_euler();
    break;
  default:
    break;
  }
}

glm::mat4 camera::get_view_matrix()
{
  switch (this->cam_type)
  {
  case camera_type::spherical:
    return glm::lookAt(this->position, this->center, this->world_up);
  case camera_type::aircraft:
    return glm::lookAt(this->position, this->position + this->front, this->up);
  default:
    return glm::mat4(1.0f);
  }
}

glm::mat4 camera::get_projection_matrix(int window_width, int window_height)
{
  switch (this->proj_type)
  {
  case projection_type::perspective:
    return glm::perspective(glm::radians(this->zoom),
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

void camera::spherical_to_cartesian()
{
  this->position.x = sin(this->spherical_coordinates.theta)
                       * cos(this->spherical_coordinates.phi)
                       * this->spherical_coordinates.radius
                     + this->center.x;
  this->position.y =
    cos(this->spherical_coordinates.theta) * this->spherical_coordinates.radius
    + this->center.y;
  this->position.z = sin(spherical_coordinates.theta)
                       * sin(this->spherical_coordinates.phi)
                       * this->spherical_coordinates.radius
                     + this->center.z;
}

// For aircraft camera
void camera::update_camera_euler()
{
  // calculate the new Front vector
  glm::vec3 new_front;
  new_front.x =
    cos(glm::radians(euler_angles.yaw)) * cos(glm::radians(euler_angles.pitch));
  new_front.y = sin(glm::radians(euler_angles.pitch));
  new_front.z =
    sin(glm::radians(euler_angles.yaw)) * cos(glm::radians(euler_angles.pitch));
  this->front = glm::normalize(new_front);
  // also re-calculate the Right and Up vector
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up = glm::normalize(glm::cross(this->right, this->front));
}

camera::camera_type camera::get_camera_type()
{
  return this->cam_type;
}

void camera::set_camera_type(camera::camera_type camera_type)
{
  this->cam_type = camera_type;
}

camera::projection_type camera::get_projection_type()
{
  return this->proj_type;
}

void camera::set_projection_type(projection_type projection_type)
{
  this->proj_type = projection_type;
}

glm::vec3 camera::get_position()
{
  return this->position;
}

void camera::set_position(glm::vec3 position)
{
  this->position = position;
}

glm::vec3 camera::get_world_up()
{
  return this->world_up;
}

void camera::set_world_up(glm::vec3 world_up)
{
  this->world_up = world_up;
}

glm::vec3 camera::get_center()
{
  return this->center;
}

void camera::set_center(glm::vec3 center)
{
  this->center = center;
}

float camera::get_movement_speed()
{
  return this->movement_speed;
}

void camera::set_movement_speed(float movement_speed)
{
  this->movement_speed = movement_speed;
}

float camera::get_mouse_sensitivity()
{
  return this->mouse_sensitivity;
}

void camera::set_mouse_sensitivity(float mouse_sensitivity)
{
  this->mouse_sensitivity = mouse_sensitivity;
}

float camera::get_zoom()
{
  return this->zoom;
}

void camera::set_zoom(float zoom)
{
  this->zoom = zoom;
}

types::spherical_coordinates camera::get_spherical_coordinates()
{
  return this->spherical_coordinates;
}

void camera::set_spherical_coordinates(
  types::spherical_coordinates spherical_coordinates)
{
  this->spherical_coordinates = spherical_coordinates;
}

types::euler_angles camera::get_euler_angles()
{
  return this->euler_angles;
}

void camera::set_euler_angles(types::euler_angles euler_angles)
{
  this->euler_angles = euler_angles;
}

glm::vec3 camera::get_front()
{
  return this->front;
}

void camera::set_front(glm::vec3 front)
{
  this->front = front;
}

glm::vec3 camera::get_up()
{
  return this->up;
}

void camera::set_up(glm::vec3 up)
{
  this->up = up;
}

glm::vec3 camera::get_right()
{
  return this->right;
}

void camera::set_right(glm::vec3 right)
{
  this->right = right;
}

bool camera::get_first_mouse()
{
  return this->first_mouse;
}

void camera::set_first_mouse(bool first_mouse)
{
  this->first_mouse = first_mouse;
}

float camera::get_last_x()
{
  return this->last_x;
}

void camera::set_last_x(float last_x)
{
  this->last_x = last_x;
}

float camera::get_last_y()
{
  return this->last_y;
}

void camera::set_last_y(float last_y)
{
  this->last_y = last_y;
}

//
// Builder
//

camera::builder &
camera::builder::camera_type(camera::camera_type camera_type)
{
  this->camera_type_val = camera_type;
  return *this;
}

camera::builder &
camera::builder::projection_type(camera::projection_type projection_type)
{
  this->projection_type_val = projection_type;
  return *this;
}

camera::builder &camera::builder::position(glm::vec3 position)
{
  this->position_val = position;
  return *this;
}

camera::builder &camera::builder::world_up(glm::vec3 world_up)
{
  this->world_up_val = world_up;
  return *this;
}

camera::builder &camera::builder::center(glm::vec3 center)
{
  this->center_val = center;
  return *this;
}

camera::builder &camera::builder::movement_speed(float movement_speed)
{
  this->movement_speed_val = movement_speed;
  return *this;
}

camera::builder &camera::builder::mouse_sensitivity(float mouse_sensitivity)
{
  this->mouse_sensitivity_val = mouse_sensitivity;
  return *this;
}

camera::builder &camera::builder::zoom(float zoom)
{
  this->zoom_val = zoom;
  return *this;
}

camera::builder &camera::builder::spherical_coordinates(
  types::spherical_coordinates spherical_coordinates)
{
  this->spherical_coordinates_val = spherical_coordinates;
  return *this;
}

camera::builder &
camera::builder::euler_angles(types::euler_angles euler_angles)
{
  this->euler_angles_val = euler_angles;
  return *this;
}

camera::builder &camera::builder::front(glm::vec3 front)
{
  this->front_val = front;
  return *this;
}

camera::builder &camera::builder::up(glm::vec3 up)
{
  this->up_val = up;
  return *this;
}

camera::builder &camera::builder::right(glm::vec3 right)
{
  this->right_val = right;
  return *this;
}

brenta::camera camera::camera::builder::build()
{
  return camera(camera_type_val, projection_type_val, position_val,
                world_up_val, center_val, movement_speed_val,
                mouse_sensitivity_val, zoom_val,
                spherical_coordinates_val, euler_angles_val,
                front_val, up_val, right_val);
}
