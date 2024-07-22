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

struct SphericalCoordinates {
    float theta;
    float phi;
    float radius;
};

struct EulerAngles {
    float yaw;
    float pitch;
    float roll;
};

} // namespace Types

class Camera
{
public:
    /* camera Attributes */
    static glm::vec3 Position;
    static glm::vec3 WorldUp;
    static glm::vec3 center;

    /* camera options */
    static float MovementSpeed;
    static float MouseSensitivity;
    static float Zoom;        // Field of view
    static float lastX;       // Last mouse position x
    static float lastY;       // Last mouse position y
    static bool  firstMouse;  // is first mouse movement

    /* Spherical coordinates for the camera */
    static Types::SphericalCoordinates sphericalCoordinates;
 
    /* euler Angles */
    // Only for aircraft camera
    static Types::EulerAngles eulerAngles;
    static glm::vec3 Front;
    static glm::vec3 Up;
    static glm::vec3 Right;

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
    static void updateCameraEuler();   // euclids angles
    static void SphericalToCartesian(); // sperical coordinates
};

} // namespace ECS

#endif
