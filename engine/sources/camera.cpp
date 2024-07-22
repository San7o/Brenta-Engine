#include "camera.h"
#include "screen.h"
#include "engine_logger.h"

#include <cmath>

using namespace ECS;

/* camera Attributes */
glm::vec3 Camera::Position = glm::vec3(0.0f, 5.0f, 14.0f);
glm::vec3 Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::Up;
glm::vec3 Camera::Right;
glm::vec3 Camera::WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::center = glm::vec3(0.0f, 5.0f, 0.0f);
/* euler Angles */
float Camera::Yaw = -90.0f;
float Camera::Pitch = -45.0f;
/* camera options */
float Camera::MovementSpeed = 2.5f;
float Camera::MouseSensitivity = 0.05f;
float Camera::Zoom = 45.0f;
float Camera::lastX;
float Camera::lastY;
bool  Camera::firstMouse = true;
float rotX = 0.0f;

void Camera::Init()
{
    Camera::updateCameraVectors();
    Logger::Log(Types::LogLevel::INFO, "Camera initialized");
}

glm::mat4 Camera::GetViewMatrix()
{
    //return glm::lookAt(Position, Position + Front, Up);
    return glm::lookAt(Position, center, WorldUp);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(Zoom),
        (float) ECS::Screen::GetWidth() / (float) ECS::Screen::GetHeight(),
        0.1f, 100.0f);
}

void Camera::ProcessKeyboard(Types::CameraMovement direction, float deltaTime)
{
    /*
    float velocity = MovementSpeed * deltaTime;
    if (direction == Types::CameraMovement::FORWARD)
        Position += Front * velocity;
    if (direction == Types::CameraMovement::BACKWARD)
        Position -= Front * velocity;
    if (direction == Types::CameraMovement::LEFT)
        Position -= Right * velocity;
    if (direction == Types::CameraMovement::RIGHT)
        Position += Right * velocity;
    */
}

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


        /*
        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        */

        // Current angle in radians
        const float radius = glm::length(Position - center);
        rotX += xoffset * MouseSensitivity;

        float camX = sin(rotX) * radius + center.x;
        float camZ = cos(rotX) * radius + center.z;

        Position = glm::vec3(camX, Position.y, camZ);
    }
    /* move the camera */
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

        xoffset *= MouseSensitivity * 0.1f;
        yoffset *= MouseSensitivity * 0.1f;

        Position += glm::vec3(-xoffset, -yoffset, 0.0f);
        center += glm::vec3(-xoffset, -yoffset, 0.0f);
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

/* For aircraft camera */
void Camera::updateCameraVectors()
{
    /* calculate the new Front vector */
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
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
    return Yaw;
}

float Camera::GetPitch()
{
    return Pitch;
}
