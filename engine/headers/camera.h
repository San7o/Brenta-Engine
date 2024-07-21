#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECS {

namespace Types {

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

} // namespace Types

class Camera
{
public:
    /* camera Attributes */
    static glm::vec3 Position;
    static glm::vec3 Front;
    static glm::vec3 Up;
    static glm::vec3 Right;
    static glm::vec3 WorldUp;
    /* euler Angles */
    static float Yaw;
    static float Pitch;
    /* camera options */
    static float MovementSpeed;
    static float MouseSensitivity;
    static float Zoom;
    static float lastX;
    static float lastY;
    static bool firstMouse;

    /* constructor with vectors */
    Camera() = delete;
    static void Init();

    static glm::vec3 GetPosition();
    static float GetYaw();
    static float GetPitch();

    static glm::mat4 GetViewMatrix();
    static glm::mat4 GetProjectionMatrix();
    static void ProcessKeyboard(Types::CameraMovement direction,
                    float deltaTime);
    static void ProcessMouseMovement(double xpos, double ypos,
                    GLboolean constrainPitch = true);
    static void ProcessMouseScroll(float yoffset);

private:
    static void updateCameraVectors();
};

} // namespace ECS

#endif
