// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/mouse.hpp>
#include <brenta/input.hpp>
#include <brenta/window.hpp>

#include <demo/callbacks/camera_mouse.hpp>

using namespace brenta;

void init_camera_mouse_callback(tenno::weak_ptr<Camera> cam, Mouse *mouse)
{
  auto camera_mouse_callback = [cam, mouse](double xpos, double ypos)
  {
    // Rotate the camera
    if (Window::is_key_pressed(Key::LeftShift))
    {
      if (mouse->get_first())
      {
        mouse->set_x(xpos);
        mouse->set_y(ypos);
        mouse->set_first(false);
      }

      float xoffset = xpos - mouse->get_x();
      float yoffset = mouse->get_y() - ypos;
      mouse->set_x(xpos);
      mouse->set_y(ypos);

      auto sensitivity = mouse->get_sensitivity();
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      auto camera = cam.lock();
      if (!camera) return;
      
      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);
        scam.theta += yoffset * sensitivity;
        scam.phi += xoffset * sensitivity;

        if (scam.theta <= 0.01f) scam.theta = 0.01f;
        if (scam.theta >= 3.13f) scam.theta = 3.13f;

        camera->set_pos(scam);
        
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    // translate the cam center
    else if (Window::is_key_pressed(Key::LeftControl))
    {
      if (mouse->get_first())
      {
        mouse->set_x(xpos);
        mouse->set_y(ypos);
        mouse->set_first(false);
      }

      float xoffset = xpos - mouse->get_x();
      float yoffset = mouse->get_y() - ypos;
      mouse->set_x(xpos);
      mouse->set_y(ypos);

      auto sensitivity = mouse->get_sensitivity();
      xoffset *= sensitivity * 0.3f;
      yoffset *= sensitivity * 0.3f;

      auto camera = cam.lock();
      if (!camera) return;

      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);
        glm::vec3 world_pos = camera->get_transform().get_pos();
        // Local coordinate system
        glm::vec3 fixed_center =
          glm::vec3(scam.center.x, world_pos.y, scam.center.z);
        // Unit vectors
        glm::vec3 front =
          glm::normalize(world_pos - fixed_center);
        glm::vec3 right =
          glm::normalize(glm::cross(front, camera->get_world_up()));

        scam.center += right * glm::vec3(xoffset);
        scam.center -= camera->get_world_up() * glm::vec3(yoffset);

        camera->set_pos(scam);
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    // zoom the camera
    else if (Window::is_key_pressed(Key::LeftAlt))
    {
      if (mouse->get_first())
      {
        mouse->set_x(xpos);
        mouse->set_y(ypos);
        mouse->set_first(false);
      }

      float xoffset = xpos - mouse->get_x();
      float yoffset = mouse->get_y() - ypos;
      mouse->set_x(xpos);
      mouse->set_y(ypos);

      xoffset *= mouse->get_sensitivity();
      yoffset *= mouse->get_sensitivity();

      auto camera = cam.lock();
      if (!camera) return;
      
      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);

        scam.radius -= yoffset;
        if (scam.radius <= 0.1f) scam.radius = 0.1f;

        camera->set_pos(scam);
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    else
    {
      mouse->set_first(true);
    }
  };
  Input::add_mouse_callback("CameraCallback", camera_mouse_callback);
}
