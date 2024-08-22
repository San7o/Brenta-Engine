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

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECS
{

namespace Types
{

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct SphericalCoordinates
{
    float theta;
    float phi;
    float radius;
};

struct EulerAngles
{
    float yaw;
    float pitch;
    float roll;
};

} // namespace Types

namespace Enums
{

enum CameraType
{
    AIRCRAFT,
    SPHERICAL
};

enum ProjectionType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

} // namespace Enums

class Camera
{
public:
    /* select which coordinate system to use */
    static Enums::CameraType CameraType;
    static Enums::ProjectionType ProjectionType;
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

    static void updateCameraEuler();   // euclids angles
    static void SphericalToCartesian(); // sperical coordinates
};

} // namespace ECS
