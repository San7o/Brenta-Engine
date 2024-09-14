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

#include "callbacks/camera_mouse_callback.hpp"

#include "engine.hpp"

using namespace brenta;

void init_camera_mouse_callback()
{
    auto camera_mouse_callback = [](double xpos, double ypos)
    {
        /* Rotate the camera */
        if (screen::is_key_pressed(GLFW_KEY_LEFT_SHIFT))
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
        else if (screen::is_key_pressed(GLFW_KEY_LEFT_CONTROL))
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
                glm::vec3(default_camera.center.x, default_camera.position.y, default_camera.center.z);
            glm::vec3 front =
                glm::normalize(default_camera.position - fixed_center); // Versor
            glm::vec3 right =
                glm::normalize(glm::cross(front, default_camera.world_up)); // Versor

            default_camera.center += right * glm::vec3(xoffset);
            default_camera.center -= default_camera.world_up * glm::vec3(yoffset);
            default_camera.spherical_to_cartesian();
        }
        /* zoom the default_camera */
        else if (screen::is_key_pressed(GLFW_KEY_LEFT_ALT))
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
