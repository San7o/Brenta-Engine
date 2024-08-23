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

#include "camera.hpp"
#include "screen.hpp"
#include "engine_logger.hpp"

#include <cmath>

using namespace Brenta;
using namespace Brenta::Utils;

Camera::Camera(Enums::CameraType camera_type,
               Enums::ProjectionType projection_type,
               glm::vec3 position,
               glm::vec3 worldUp,
               glm::vec3 center,
               float movementSpeed,
               float mouseSensitivity,
               float zoom,
               Types::SphericalCoordinates sphericalCoordinates,
               Types::EulerAngles eulerAngles,
               glm::vec3 front,
               glm::vec3 up,
               glm::vec3 right)
{
    this->CameraType = camera_type;
    this->ProjectionType = projection_type;
    this->Position = position;
    this->WorldUp = worldUp;
    this->center = center;
    this->MovementSpeed = movementSpeed;
    this->MouseSensitivity = mouseSensitivity;
    this->Zoom = zoom;
    this->sphericalCoordinates = sphericalCoordinates;
    this->eulerAngles = eulerAngles;
    this->Front = front;
    this->Up = up;
    this->Right = right;

    /* Update the camera */
    switch (this->CameraType)
    {
        case Enums::CameraType::SPHERICAL:
            SphericalToCartesian();
            break;
        case Enums::CameraType::AIRCRAFT:
            updateCameraEuler();
            break;
        default:
            break;
    }

    INFO("Camera created");
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
                (float) Screen::GetWidth() / (float) Screen::GetHeight(),
                0.1f, 1000.0f);
        case Enums::ProjectionType::ORTHOGRAPHIC:
            return glm::ortho((float) -Screen::GetWidth() / 2.0f,
                              (float)  Screen::GetWidth() / 2.0f,
                              (float) -Screen::GetHeight() / 2.0f,
                              (float)  Screen::GetHeight() / 2.0f,
                              0.1f, 100.0f);
        default:
            return glm::mat4(1.0f);
    }
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

Enums::CameraType Camera::GetCameraType()
{
    return CameraType;
}

void Camera::SetCameraType(Enums::CameraType cameraType)
{
    Camera::CameraType = cameraType;
}

Enums::ProjectionType Camera::GetProjectionType()
{
    return ProjectionType;
}

void Camera::SetProjectionType(Enums::ProjectionType projectionType)
{
    Camera::ProjectionType = projectionType;
}

glm::vec3 Camera::GetPosition()
{
    return Position;
}

void Camera::SetPosition(glm::vec3 position)
{
    Camera::Position = position;
}

glm::vec3 Camera::GetWorldUp()
{
    return WorldUp;
}

void Camera::SetWorldUp(glm::vec3 worldUp)
{
    Camera::WorldUp = worldUp;
}

glm::vec3 Camera::GetCenter()
{
    return center;
}

void Camera::SetCenter(glm::vec3 center)
{
    Camera::center = center;
}

float Camera::GetMovementSpeed()
{
    return MovementSpeed;
}

void Camera::SetMovementSpeed(float movementSpeed)
{
    Camera::MovementSpeed = movementSpeed;
}

float Camera::GetMouseSensitivity()
{
    return MouseSensitivity;
}

void Camera::SetMouseSensitivity(float mouseSensitivity)
{
    Camera::MouseSensitivity = mouseSensitivity;
}

float Camera::GetZoom()
{
    return Zoom;
}

void Camera::SetZoom(float zoom)
{
    Camera::Zoom = zoom;
}

Types::SphericalCoordinates Camera::GetSphericalCoordinates()
{
    return sphericalCoordinates;
}

void Camera::SetSphericalCoordinates(Types::SphericalCoordinates sphericalCoordinates)
{
    Camera::sphericalCoordinates = sphericalCoordinates;
}

Types::EulerAngles Camera::GetEulerAngles()
{
    return eulerAngles;
}

void Camera::SetEulerAngles(Types::EulerAngles eulerAngles)
{
    Camera::eulerAngles = eulerAngles;
}

glm::vec3 Camera::GetFront()
{
    return Front;
}

void Camera::SetFront(glm::vec3 front)
{
    Camera::Front = front;
}

glm::vec3 Camera::GetUp()
{
    return Up;
}

void Camera::SetUp(glm::vec3 up)
{
    Camera::Up = up;
}

glm::vec3 Camera::GetRight()
{
    return Right;
}

void Camera::SetRight(glm::vec3 right)
{
    Camera::Right = right;
}

bool Camera::GetFirstMouse()
{
    return firstMouse;
}

void Camera::SetFirstMouse(bool firstMouse)
{
    Camera::firstMouse = firstMouse;
}

float Camera::GetLastX()
{
    return lastX;
}

void Camera::SetLastX(float lastX)
{
    Camera::lastX = lastX;
}

float Camera::GetLastY()
{
    return lastY;
}

void Camera::SetLastY(float lastY)
{
    Camera::lastY = lastY;
}

Camera::Builder& Camera::Builder::set_camera_type(Enums::CameraType camera_type)
{
    this->camera_type = camera_type;
    return *this;
}

Camera::Builder& Camera::Builder::set_projection_type(Enums::ProjectionType projection_type)
{
    this->projection_type = projection_type;
    return *this;
}

Camera::Builder& Camera::Builder::set_position(glm::vec3 position)
{
    this->position = position;
    return *this;
}

Camera::Builder& Camera::Builder::set_world_up(glm::vec3 worldUp)
{
    this->worldUp = worldUp;
    return *this;
}

Camera::Builder& Camera::Builder::set_center(glm::vec3 center)
{
    this->center = center;
    return *this;
}

Camera::Builder& Camera::Builder::set_movement_speed(float movementSpeed)
{
    this->movementSpeed = movementSpeed;
    return *this;
}

Camera::Builder& Camera::Builder::set_mouse_sensitivity(float mouseSensitivity)
{
    this->mouseSensitivity = mouseSensitivity;
    return *this;
}

Camera::Builder& Camera::Builder::set_zoom(float zoom)
{
    this->zoom = zoom;
    return *this;
}

Camera::Builder& Camera::Builder::set_spherical_coordinates(Types::SphericalCoordinates sphericalCoordinates)
{
    this->sphericalCoordinates = sphericalCoordinates;
    return *this;
}

Camera::Builder& Camera::Builder::set_eulerAngles(Types::EulerAngles eulerAngles)
{
    this->eulerAngles = eulerAngles;
    return *this;
}

Camera::Builder& Camera::Builder::set_front(glm::vec3 front)
{
    this->front = front;
    return *this;
}

Camera::Builder& Camera::Builder::set_up(glm::vec3 up)
{
    this->up = up;
    return *this;
}

Camera::Builder& Camera::Builder::set_right(glm::vec3 right)
{
    this->right = right;
    return *this;
}

Camera Camera::Camera::Builder::build()
{
    return Camera(camera_type, projection_type, position, worldUp, center,
                  movementSpeed, mouseSensitivity, zoom, sphericalCoordinates,
                  eulerAngles, front, up, right);
}
