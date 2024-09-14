/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "camera.hpp"

#include "engine_logger.hpp"
#include "screen.hpp"

#include <cmath>

using namespace brenta;

camera::camera(enums::camera_type camera_type,
               enums::projection_type projection_type, glm::vec3 position,
               glm::vec3 world_up, glm::vec3 center, float movement_speed,
               float mouse_sensitivity, float zoom,
               types::spherical_coordinates spherical_coordinates,
               types::euler_angles euler_angles, glm::vec3 front, glm::vec3 up,
               glm::vec3 right)
{
    this->camera_type           = camera_type;
    this->projection_type       = projection_type;
    this->position              = position;
    this->world_up              = world_up;
    this->center                = center;
    this->movement_speed        = movement_speed;
    this->mouse_sensitivity     = mouse_sensitivity;
    this->zoom                  = zoom;
    this->spherical_coordinates = spherical_coordinates;
    this->euler_angles          = euler_angles;
    this->front                 = front;
    this->up                    = up;
    this->right                 = right;

    /* Update the camera */
    switch (this->camera_type)
    {
    case enums::camera_type::SPHERICAL:
        spherical_to_cartesian();
        break;
    case enums::camera_type::AIRCRAFT:
        update_camera_euler();
        break;
    default:
        break;
    }

    INFO("Camera created");
}

glm::mat4 camera::get_view_matrix()
{
    switch (camera_type)
    {
    case enums::camera_type::SPHERICAL:
        return glm::lookAt(this->position, this->center, this->world_up);
    case enums::camera_type::AIRCRAFT:
        return glm::lookAt(this->position, this->position + this->front, this->up);
    default:
        return glm::mat4(1.0f);
    }
}

glm::mat4 camera::get_projection_matrix()
{
    switch (this->projection_type)
    {
    case enums::projection_type::PERSPECTIVE:
        return glm::perspective(glm::radians(this->zoom),
                                (float) screen::get_width()
                                 / (float) screen::get_height(),
                                0.1f, 1000.0f);
    case enums::projection_type::ORTHOGRAPHIC:
        return glm::ortho((float) -screen::get_width() / 2.0f,
                          (float) screen::get_width() / 2.0f,
                          (float) -screen::get_height() / 2.0f,
                          (float) screen::get_height() / 2.0f, 0.1f, 100.0f);
    default:
        return glm::mat4(1.0f);
    }
}

void camera::spherical_to_cartesian()
{
    this->position.x = sin(this->spherical_coordinates.theta) * cos(this->spherical_coordinates.phi)
                     * this->spherical_coordinates.radius
                 + this->center.x;
    this->position.y = cos(this->spherical_coordinates.theta) * this->spherical_coordinates.radius
                 + this->center.y;
    this->position.z = sin(spherical_coordinates.theta) * sin(this->spherical_coordinates.phi)
                     * this->spherical_coordinates.radius
                 + this->center.z;
}

/* For aircraft camera */
void camera::update_camera_euler()
{
    /* calculate the new Front vector */
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(euler_angles.yaw))
              * cos(glm::radians(euler_angles.pitch));
    new_front.y = sin(glm::radians(euler_angles.pitch));
    new_front.z = sin(glm::radians(euler_angles.yaw))
              * cos(glm::radians(euler_angles.pitch));
    this->front = glm::normalize(new_front);
    /* also re-calculate the Right and Up vector */
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

enums::camera_type camera::get_camera_type()
{
    return this->camera_type;
}

void camera::set_camera_type(enums::camera_type camera_type)
{
    this->camera_type = camera_type;
}

enums::projection_type camera::get_projection_type()
{
    return this->projection_type;
}

void camera::set_projection_type(enums::projection_type projection_type)
{
    this->projection_type = projection_type;
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
    camera::euler_angles = euler_angles;
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

camera::builder &camera::builder::set_camera_type(enums::camera_type camera_type)
{
    this->camera_type = camera_type;
    return *this;
}

camera::builder &
camera::builder::set_projection_type(enums::projection_type projection_type)
{
    this->projection_type = projection_type;
    return *this;
}

camera::builder &camera::builder::set_position(glm::vec3 position)
{
    this->position = position;
    return *this;
}

camera::builder &camera::builder::set_world_up(glm::vec3 world_up)
{
    this->world_up = world_up;
    return *this;
}

camera::builder &camera::builder::set_center(glm::vec3 center)
{
    this->center = center;
    return *this;
}

camera::builder &camera::builder::set_movement_speed(float movement_speed)
{
    this->movement_speed = movement_speed;
    return *this;
}

camera::builder &camera::builder::set_mouse_sensitivity(float mouse_sensitivity)
{
    this->mouse_sensitivity = mouse_sensitivity;
    return *this;
}

camera::builder &camera::builder::set_zoom(float zoom)
{
    this->zoom = zoom;
    return *this;
}

camera::builder &camera::builder::set_spherical_coordinates(
    types::spherical_coordinates spherical_coordinates)
{
    this->spherical_coordinates = spherical_coordinates;
    return *this;
}

camera::builder &
camera::builder::set_euler_angles(types::euler_angles euler_angles)
{
    this->euler_angles = euler_angles;
    return *this;
}

camera::builder &camera::builder::set_front(glm::vec3 front)
{
    this->front = front;
    return *this;
}

camera::builder &camera::builder::set_up(glm::vec3 up)
{
    this->up = up;
    return *this;
}

camera::builder &camera::builder::set_right(glm::vec3 right)
{
    this->right = right;
    return *this;
}

brenta::camera camera::camera::builder::build()
{
    return camera(camera_type, projection_type, position, world_up, center,
                  movement_speed, mouse_sensitivity, zoom, spherical_coordinates,
                  euler_angles, front, up, right);
}
