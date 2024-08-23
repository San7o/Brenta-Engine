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

using namespace Brenta;

void InitCameraMouseCallback()
{
    auto camera_mouse_callback = [](double xpos, double ypos)
    {
        /* Rotate the camera */
        if (Screen::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        {
            if (camera.firstMouse)
            {
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = false;
            }

            float xoffset = xpos - camera.lastX;
            float yoffset = camera.lastY - ypos;
            camera.lastX = xpos;
            camera.lastY = ypos;

            xoffset *= camera.MouseSensitivity;
            yoffset *= camera.MouseSensitivity;

            camera.sphericalCoordinates.theta +=
                yoffset * camera.MouseSensitivity;
            camera.sphericalCoordinates.phi +=
                xoffset * camera.MouseSensitivity;

            if (camera.sphericalCoordinates.theta <= 0.01f)
                camera.sphericalCoordinates.theta = 0.01f;
            if (camera.sphericalCoordinates.theta >= 3.13f)
                camera.sphericalCoordinates.theta = 3.13f;

            camera.SphericalToCartesian();
        }
        /* translate the camera center */
        else if (Screen::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        {
            if (camera.firstMouse)
            {
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = false;
            }

            float xoffset = xpos - camera.lastX;
            float yoffset = camera.lastY - ypos;
            camera.lastX = xpos;
            camera.lastY = ypos;

            xoffset *= camera.MouseSensitivity * 0.3f;
            yoffset *= camera.MouseSensitivity * 0.3f;

            // Local coordinate system
            glm::vec3 fixed_center =
                glm::vec3(camera.center.x, camera.Position.y, camera.center.z);
            glm::vec3 front =
                glm::normalize(camera.Position - fixed_center); // Versor
            glm::vec3 right =
                glm::normalize(glm::cross(front, camera.WorldUp)); // Versor

            camera.center += right * glm::vec3(xoffset);
            camera.center -= camera.WorldUp * glm::vec3(yoffset);
            camera.SphericalToCartesian();
        }
        /* zoom the camera */
        else if (Screen::isKeyPressed(GLFW_KEY_LEFT_ALT))
        {
            if (camera.firstMouse)
            {
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = false;
            }

            float xoffset = xpos - camera.lastX;
            float yoffset = camera.lastY - ypos;
            camera.lastX = xpos;
            camera.lastY = ypos;

            xoffset *= camera.MouseSensitivity;
            yoffset *= camera.MouseSensitivity;

            camera.sphericalCoordinates.radius -= yoffset;
            if (camera.sphericalCoordinates.radius <= 0.1f)
                camera.sphericalCoordinates.radius = 0.1f;
            camera.SphericalToCartesian();
        }
        else
        {
            camera.firstMouse = true;
        }
    };
    Input::AddMousePosCallback("CameraCallback", camera_mouse_callback);
}
