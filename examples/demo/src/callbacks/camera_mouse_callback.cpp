// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/callbacks/camera_mouse_callback.hpp>

using namespace brenta;

void init_camera_mouse_callback()
{
  auto camera_mouse_callback = [](double xpos, double ypos)
  {
    /* Rotate the camera */
    if (window::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
    {
      if (default_camera.first_mouse)
      {
        default_camera.last_x = xpos;
        default_camera.last_y = ypos;
        default_camera.first_mouse = false;
      }

      float xoffset = xpos - default_camera.last_x;
      float yoffset = default_camera.last_y - ypos;
      default_camera.last_x = xpos;
      default_camera.last_y = ypos;

      xoffset *= default_camera.mouse_sensitivity;
      yoffset *= default_camera.mouse_sensitivity;

      default_camera.spherical_coordinates.theta +=
        yoffset * default_camera.mouse_sensitivity;
      default_camera.spherical_coordinates.phi +=
        xoffset * default_camera.mouse_sensitivity;

      if (default_camera.spherical_coordinates.theta <= 0.01f)
        default_camera.spherical_coordinates.theta = 0.01f;
      if (default_camera.spherical_coordinates.theta >= 3.13f)
        default_camera.spherical_coordinates.theta = 3.13f;

      default_camera.spherical_to_cartesian();
    }
    /* translate the default_camera center */
    else if (window::is_key_pressed(GLFW_KEY_LEFT_CONTROL))
    {
      if (default_camera.first_mouse)
      {
        default_camera.last_x = xpos;
        default_camera.last_y = ypos;
        default_camera.first_mouse = false;
      }

      float xoffset = xpos - default_camera.last_x;
      float yoffset = default_camera.last_y - ypos;
      default_camera.last_x = xpos;
      default_camera.last_y = ypos;

      xoffset *= default_camera.mouse_sensitivity * 0.3f;
      yoffset *= default_camera.mouse_sensitivity * 0.3f;

      // Local coordinate system
      glm::vec3 fixed_center =
        glm::vec3(default_camera.center.x, default_camera.position.y,
                  default_camera.center.z);
      glm::vec3 front =
        glm::normalize(default_camera.position - fixed_center); // Versor
      glm::vec3 right =
        glm::normalize(glm::cross(front, default_camera.world_up)); // Versor

      default_camera.center += right * glm::vec3(xoffset);
      default_camera.center -= default_camera.world_up * glm::vec3(yoffset);
      default_camera.spherical_to_cartesian();
    }
    /* zoom the default_camera */
    else if (window::is_key_pressed(GLFW_KEY_LEFT_ALT))
    {
      if (default_camera.first_mouse)
      {
        default_camera.last_x = xpos;
        default_camera.last_y = ypos;
        default_camera.first_mouse = false;
      }

      float xoffset = xpos - default_camera.last_x;
      float yoffset = default_camera.last_y - ypos;
      default_camera.last_x = xpos;
      default_camera.last_y = ypos;

      xoffset *= default_camera.mouse_sensitivity;
      yoffset *= default_camera.mouse_sensitivity;

      default_camera.spherical_coordinates.radius -= yoffset;
      if (default_camera.spherical_coordinates.radius <= 0.1f)
        default_camera.spherical_coordinates.radius = 0.1f;
      default_camera.spherical_to_cartesian();
    }
    else
    {
      default_camera.first_mouse = true;
    }
  };
  input::add_mouse_pos_callback("CameraCallback", camera_mouse_callback);
}
