// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/callbacks/camera_mouse.hpp>

using namespace brenta;

void init_camera_mouse_callback(camera *cam)
{
  auto camera_mouse_callback = [cam](double xpos, double ypos)
  {
    // Rotate the camera
    if (window::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
    {
      if (cam->first_mouse)
      {
        cam->last_x = xpos;
        cam->last_y = ypos;
        cam->first_mouse = false;
      }

      float xoffset = xpos - cam->last_x;
      float yoffset = cam->last_y - ypos;
      cam->last_x = xpos;
      cam->last_y = ypos;

      xoffset *= cam->mouse_sensitivity;
      yoffset *= cam->mouse_sensitivity;

      cam->spherical_coordinates.theta +=
        yoffset * cam->mouse_sensitivity;
      cam->spherical_coordinates.phi +=
        xoffset * cam->mouse_sensitivity;

      if (cam->spherical_coordinates.theta <= 0.01f)
        cam->spherical_coordinates.theta = 0.01f;
      if (cam->spherical_coordinates.theta >= 3.13f)
        cam->spherical_coordinates.theta = 3.13f;

      cam->spherical_to_cartesian();
    }
    /* translate the cam center */
    else if (window::is_key_pressed(GLFW_KEY_LEFT_CONTROL))
    {
      if (cam->first_mouse)
      {
        cam->last_x = xpos;
        cam->last_y = ypos;
        cam->first_mouse = false;
      }

      float xoffset = xpos - cam->last_x;
      float yoffset = cam->last_y - ypos;
      cam->last_x = xpos;
      cam->last_y = ypos;

      xoffset *= cam->mouse_sensitivity * 0.3f;
      yoffset *= cam->mouse_sensitivity * 0.3f;

      // Local coordinate system
      glm::vec3 fixed_center =
        glm::vec3(cam->center.x, cam->position.y,
                  cam->center.z);
      glm::vec3 front =
        glm::normalize(cam->position - fixed_center); // Versor
      glm::vec3 right =
        glm::normalize(glm::cross(front, cam->world_up)); // Versor

      cam->center += right * glm::vec3(xoffset);
      cam->center -= cam->world_up * glm::vec3(yoffset);
      cam->spherical_to_cartesian();
    }
    /* zoom the camera */
    else if (window::is_key_pressed(GLFW_KEY_LEFT_ALT))
    {
      if (cam->first_mouse)
      {
        cam->last_x = xpos;
        cam->last_y = ypos;
        cam->first_mouse = false;
      }

      float xoffset = xpos - cam->last_x;
      float yoffset = cam->last_y - ypos;
      cam->last_x = xpos;
      cam->last_y = ypos;

      xoffset *= cam->mouse_sensitivity;
      yoffset *= cam->mouse_sensitivity;

      cam->spherical_coordinates.radius -= yoffset;
      if (cam->spherical_coordinates.radius <= 0.1f)
        cam->spherical_coordinates.radius = 0.1f;
      cam->spherical_to_cartesian();
    }
    else
    {
      cam->first_mouse = true;
    }
  };
  input::add_mouse_pos_callback("CameraCallback", camera_mouse_callback);
}
