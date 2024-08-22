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

/**
 * @brief Camera movement directions
 *
 * Used to define the direction of the camera movement
 * when reacting to keyboard input.
 */
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/**
 * @brief Spherical coordinates
 *
 * This struct represents the spherical coordinates of a point
 * in 3D space.
 */
struct SphericalCoordinates
{
    float theta;
    float phi;
    float radius;
};

/**
 * @brief Euler angles
 *
 * This struct represents the euler angles of a rigid body
 * in 3D space.
 */
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

/**
 * @brief The Camera class
 *
 * This class represents a camera in the 3D world. It is used to
 * define the position, orientation and projection of the camera.
 * The class provides a Builder pattern to create a camera object,
 * and a default global camera called "camera".
 *
 * ## Camera type
 * The camera can be of two types: AIRCRAFT or SPHERICAL. The
 * AIRCRAFT type is used to represent a camera that moves in the
 * world space, while the SPHERICAL type is used to represent a
 * camera that rotates around a center point. They use respectively
 * euler angles and spherical coordinates to represent the position
 * of the camera.
 *
 * ## Projection
 * The camera can have two types of projection: PERSPECTIVE or
 * ORTHOGRAPHIC.
 *
 * Each time the euler angles or the spherical coordinates are
 * updated, the camera needs to update the front, up and right
 * vectors by calling the updateCameraEuler() or SphericalToCartesian()
 * functions.
 */
class Camera
{
public:
    /**
     * @brief Projection type
     */
    Enums::ProjectionType ProjectionType;
    /**
     * @brief Camera type
     */
    Enums::CameraType CameraType;
    /**
     * @brief Camera position
     */
    glm::vec3 Position;
    /**
     * @brief The world up vector
     *
     * This vector defines the up direction in the world space.
     */
    glm::vec3 WorldUp;
    /**
     * @brief The center of the spehere
     *
     * When using a spherical camera, this is the center of the sphere
     * the camera is rotating around.
     */
    glm::vec3 center;
    /**
     * @brief Space translational movement speed
     */
    float MovementSpeed;
    /**
     * @brief Mouse sensitivity
     */
    float MouseSensitivity;
    /**
     * @brief Zoom level (field of view)
     */
    float Zoom;
    /**
     * @brief Spherical coordinates
     *
     * If the camera is of type SPHERICAL, these are the spherical
     * coordinates of the camera. Spherical coordinates are used to
     * represent the position of a point in 3D space, those are theta,
     * phi and radius.
     */
    Types::SphericalCoordinates sphericalCoordinates;
    /**
     * @brief Euler angles
     *
     * If the camera is of type AIRCRAFT, these are the euler angles
     * of the camera. Euler angles are used to represent the orientation
     * of a rigid body in 3D space, those are yaw, pitch and roll.
     */
    Types::EulerAngles eulerAngles;
    /**
     * @brief Front vector
     *
     * This vector represents the front direction of the camera.
     */
    glm::vec3 Front;
    /**
     * @brief Up vector
     *
     * This vector represents the up direction of the camera.
     */
    glm::vec3 Up;
    /**
     * @brief Right vector
     *
     * This vector represents the right direction of the camera.
     */
    glm::vec3 Right;
    /**
     * @brief Is the first mouse movement?
     *
     * This flag is used to check if the mouse movement is the first
     * movement of the camera.
     */
    bool firstMouse = true;
    /**
     * @brief Last x position of the mouse
     */
    float lastX;
    /**
     * @brief Last y position of the mouse
     */
    float lastY;

    /* Constructors */

    /**
     * @brief Default constructor
     *
     * This constructor initializes the camera with default values.
     */
    Camera() = default;
    /**
     * @brief Constructor
     *
     * Full constructor that initializes the camera with the specified values.
     */
    Camera(ECS::Enums::CameraType camera_type,
           ECS::Enums::ProjectionType projection_type,
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
           glm::vec3 right);

    /**
     * @brief Builder pattern
     *
     * This class is used to build a camera object. It is used to create
     * a camera object with the desired parameters.
     */
    class Builder;

    /* Getters */

    /**
     * @brief Get the camera type
     * @return The camera type
     */
    ECS::Enums::CameraType GetCameraType();
    /**
     * @brief Get the projection type
     * @return The projection type
     */
    ECS::Enums::ProjectionType GetProjectionType();
    /**
     * @brief Get the position of the camera
     * @return The position of the camera
     */
    glm::vec3 GetPosition();
    /**
     * @brief Get the world up vector
     * @return The world up vector
     */
    glm::vec3 GetWorldUp();
    /**
     * @brief Get the center of the sphere
     * @return The center of the sphere
     */
    glm::vec3 GetCenter();
    /**
     * @brief Get the movement speed
     * @return The movement speed
     */
    float GetMovementSpeed();
    /**
     * @brief Get the mouse sensitivity
     * @return The mouse sensitivity
     */
    float GetMouseSensitivity();
    /**
     * @brief Get the zoom level
     * @return The zoom level
     */
    float GetZoom();
    /**
     * @brief Get the spherical coordinates of the camera
     * @return The spherical coordinates of the camera
     */
    Types::SphericalCoordinates GetSphericalCoordinates();
    /**
     * @brief Get the euler angles of the camera
     * @return The euler angles of the camera
     */
    Types::EulerAngles GetEulerAngles();
    /**
     * @brief Get the view matrix
     * @return The view matrix
     */
    glm::mat4 GetViewMatrix();
    /**
     * @brief Get the projection matrix
     * @return The projection matrix
     */
    glm::mat4 GetProjectionMatrix();
    /**
     * @brief Get the front vector
     * @return The front vector
     */
    glm::vec3 GetFront();
    /**
     * @brief Get the up vector
     * @return The up vector
     */
    glm::vec3 GetUp();
    /**
     * @brief Get the right vector
     * @return The right vector
     */
    glm::vec3 GetRight();
    /**
     * @brief Get the first mouse flag
     * @return boolean flag
     */
    bool GetFirstMouse();
    /**
     * @brief Get the last x position of the mouse
     * @return The last x position of the mouse
     */
    float GetLastX();
    /**
     * @brief Get the last y position of the mouse
     * @return The last y position of the mouse
     */
    float GetLastY();

    /* Setters */

    /**
     * @brief Set the camera type
     * @param camera_type The camera type
     */
    void SetCameraType(ECS::Enums::CameraType camera_type);
    /**
     * @brief Set the projection type
     * @param projection_type The projection type
     */
    void SetProjectionType(ECS::Enums::ProjectionType projection_type);
    /**
     * @brief Set the world up vector
     * @param worldUp The world up vector
     */
    void SetWorldUp(glm::vec3 worldUp);
    /**
     * @brief Set the center of the sphere
     * @param center The center of the sphere
     */
    void SetCenter(glm::vec3 center);
    /**
     * @brief Set the movement speed
     * @param movementSpeed The movement speed
     */
    void SetMovementSpeed(float movementSpeed);
    /**
     * @brief Set the mouse sensitivity
     * @param mouseSensitivity The mouse sensitivity
     */
    void SetMouseSensitivity(float mouseSensitivity);
    /**
     * @brief Set the zoom level
     * @param zoom The zoom level
     */
    void SetZoom(float zoom);
    /**
     * @brief Set the spherical coordinates of the camera
     * @param sphericalCoordinates The spherical coordinates of the camera
     */
    void SetSphericalCoordinates(Types::SphericalCoordinates sphericalCoordinates);
    /**
     * @brief Set the euler angles of the camera
     * @param eulerAngles The euler angles of the camera
     */
    void SetEulerAngles(Types::EulerAngles eulerAngles);
    /**
     * @brief Set the front vector
     * @param front The front vector
     */
    void SetFront(glm::vec3 front);
    /**
     * @brief Set the up vector
     * @param up The up vector
     */
    void SetUp(glm::vec3 up);
    /**
     * @brief Set the right vector
     * @param right The right vector
     */
    void SetRight(glm::vec3 right);
    /**
     * @brief Set the position of the camera
     * @param position The position of the camera
     */
    void SetPosition(glm::vec3 position);
    /**
     * @brief Set the first mouse flag
     * @param firstMouse The first mouse flag
     */
    void SetFirstMouse(bool firstMouse);
    /**
     * @brief Set the last x position of the mouse
     * @param lastX The last x position of the mouse
     */
    void SetLastX(float lastX);
    /**
     * @brief Set the last y position of the mouse
     * @param lastY The last y position of the mouse
     */
    void SetLastY(float lastY);

    /* Utilities */

    /**
     * @brief Update the camera euler angles
     *
     * This function needs to be called every time the
     * camera euler angles are updated. It calculates
     * the correct front, up and right vectors from
     * the euler angles.
     */
    void updateCameraEuler();
    /**
     * @brief Update the camera spherical coordinates
     *
     * This function needs to be called every time the
     * camera spherical coordinates are updated. It the
     * correct position of the camera from the spherical
     * coordinates.
     */
    void SphericalToCartesian();
};

/**
 * @brief Builder pattern for the Camera class
 */
class Camera::Builder
{
private:
    ECS::Enums::CameraType camera_type = ECS::Enums::CameraType::AIRCRAFT;
    ECS::Enums::ProjectionType projection_type = ECS::Enums::ProjectionType::PERSPECTIVE;
    glm::vec3 position     = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 worldUp      = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 center       = glm::vec3(0.0f, 0.0f, 0.0f);
    float movementSpeed    = 2.5f;
    float mouseSensitivity = 0.1f;
    float zoom             = 45.0f;
    glm::vec3 front        = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up           = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right        = glm::vec3(1.0f, 0.0f, 0.0f);
    Types::SphericalCoordinates sphericalCoordinates = {0.0f, 0.0f, 10.0f};
    Types::EulerAngles eulerAngles = {0.0f, 0.0f, 0.0f};

public:
    Builder& set_camera_type(ECS::Enums::CameraType camera_type);
    Builder& set_projection_type(ECS::Enums::ProjectionType projection_type);
    Builder& set_position(glm::vec3 position);
    Builder& set_world_up(glm::vec3 worldUp);
    Builder& set_center(glm::vec3 center);
    Builder& set_movement_speed(float movementSpeed);
    Builder& set_mouse_sensitivity(float mouseSensitivity);
    Builder& set_zoom(float zoom);
    Builder& set_spherical_coordinates(Types::SphericalCoordinates sphericalCoordinates);
    Builder& set_eulerAngles(Types::EulerAngles eulerAngles);
    Builder& set_front(glm::vec3 front);
    Builder& set_up(glm::vec3 up);
    Builder& set_right(glm::vec3 right);

    Camera build();
};

/**
 * @brief A default camera
 *
 * This is a default camera provided by the engine.
 */
extern Camera camera;

} // namespace ECS
