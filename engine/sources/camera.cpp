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
 * The above copyright notice and this permission notice shall be included in all
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

#include "camera.h"
#include "screen.h"
#include "engine_logger.h"

#include <cmath>

using namespace ECS;

/* camera Attributes */

/* camera options */
float Camera::MovementSpeed    = 2.5f;
float Camera::MouseSensitivity = 0.05f;
float Camera::Zoom             = 45.0f;
float Camera::lastX;
float Camera::lastY;
bool  Camera::firstMouse       = true;
Enums::CameraType Camera::CameraType = Enums::CameraType::SPHERICAL;
Enums::ProjectionType Camera::ProjectionType = Enums::ProjectionType::PERSPECTIVE;

// Cartesian coordinates
glm::vec3 Camera::Position = glm::vec3(9.0f, 17.0f, 30.0f);
glm::vec3 Camera::WorldUp  = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::center   = glm::vec3(0.0f, 2.0f, 0.0f);

/* Spherical coordinates for the camera */
Types::SphericalCoordinates Camera::sphericalCoordinates =
                  {1.25f, 1.25f,  glm::length(Position - center)};

/* euler Angles */
// Only needed for aircraft camera
Types::EulerAngles Camera::eulerAngles = {-90.0f, -45.0f};
glm::vec3 Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::Up;
glm::vec3 Camera::Right;

void Camera::Init()
{
    switch (CameraType)
    {
        case Enums::CameraType::SPHERICAL:
            Camera::SphericalToCartesian();
            break;
        case Enums::CameraType::AIRCRAFT:
            Camera::updateCameraEuler();
            break;
        default:
            break;
    }
    Logger::Log(Types::LogLevel::INFO, "Camera initialized");
}

glm::mat4 Camera::GetViewMatrix()
{
    switch (CameraType)
    {
        case Enums::CameraType::SPHERICAL:
            return glm::lookAt(Position, center, WorldUp);
        case Enums::CameraType::AIRCRAFT:
            return glm::lookAt(Position, Position + Front, Up);
        default:
            return glm::mat4(1.0f);
    }
}

glm::mat4 Camera::GetProjectionMatrix()
{
    switch (ProjectionType)
    {
        case Enums::ProjectionType::PERSPECTIVE:
            return glm::perspective(glm::radians(Zoom),
                (float) ECS::Screen::GetWidth() / (float) ECS::Screen::GetHeight(),
                0.1f, 1000.0f);
        case Enums::ProjectionType::ORTHOGRAPHIC:
            return glm::ortho((float) -ECS::Screen::GetWidth() / 2.0f,
                              (float) ECS::Screen::GetWidth() / 2.0f,
                              (float) -ECS::Screen::GetHeight() / 2.0f,
                              (float) ECS::Screen::GetHeight() / 2.0f,
                              0.1f, 100.0f);
        default:
            return glm::mat4(1.0f);
    }
}

void Camera::ProcessKeyboard(Types::CameraMovement direction, float deltaTime) {}

void Camera::ProcessMouseMovement(double xpos, double ypos,
                GLboolean constrainPitch)
{

    /* Rotate the camera */
    if (Screen::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
  
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
 
        sphericalCoordinates.theta += yoffset * MouseSensitivity;
        sphericalCoordinates.phi += xoffset * MouseSensitivity;

        if (sphericalCoordinates.theta <= 0.01f)
                sphericalCoordinates.theta = 0.01f;
        if (sphericalCoordinates.theta >= 3.13f)
                sphericalCoordinates.theta = 3.13f;

        SphericalToCartesian();

    }
    /* translate the camera center */
    else if (Screen::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
  
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        xoffset *= MouseSensitivity * 0.3f;
        yoffset *= MouseSensitivity * 0.3f;

        // Local coordinate system
        glm::vec3 fixed_center = glm::vec3(center.x, Position.y, center.z);
        glm::vec3 front = glm::normalize(Position - fixed_center);    // Versor
        glm::vec3 right = glm::normalize(glm::cross(front, WorldUp)); // Versor

        center += right * glm::vec3(xoffset);
        center -= WorldUp * glm::vec3(yoffset);
        SphericalToCartesian();
    }
    /* zoom the camera */
    else if (Screen::isKeyPressed(GLFW_KEY_LEFT_ALT))
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
  
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        
        sphericalCoordinates.radius -= yoffset;
        if (sphericalCoordinates.radius <= 0.1f)
            sphericalCoordinates.radius = 0.1f;
        SphericalToCartesian();
    }
    else {
        firstMouse = true;
    }
}

void Camera::ProcessMouseScroll(float yoffset)
{

    Camera::Zoom -= (float)yoffset;
    if (Camera::Zoom < 1.0f)
        Camera::Zoom = 1.0f;
    if (Camera::Zoom > 45.0f)
        Camera::Zoom = 45.0f;
}

void Camera::SphericalToCartesian()
{
    Position.x = sin(sphericalCoordinates.theta) *
                 cos(sphericalCoordinates.phi) * sphericalCoordinates.radius + 
                 center.x;
    Position.y = cos(sphericalCoordinates.theta) * 
                 sphericalCoordinates.radius + center.y;
    Position.z = sin(sphericalCoordinates.theta) *
                 sin(sphericalCoordinates.phi) * sphericalCoordinates.radius + 
                 center.z;
}

/* For aircraft camera */
void Camera::updateCameraEuler()
{
    /* calculate the new Front vector */
    glm::vec3 front;
    front.x = cos(glm::radians(eulerAngles.yaw)) * cos(glm::radians(eulerAngles.pitch));
    front.y = sin(glm::radians(eulerAngles.pitch));
    front.z = sin(glm::radians(eulerAngles.yaw)) * cos(glm::radians(eulerAngles.pitch));
    Front = glm::normalize(front);
    /* also re-calculate the Right and Up vector */
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up    = glm::normalize(glm::cross(Right, Front));
}

glm::vec3 Camera::GetPosition()
{
    return Position;
}

float Camera::GetYaw()
{
    return eulerAngles.yaw;
}

float Camera::GetPitch()
{
    return eulerAngles.pitch;
}
