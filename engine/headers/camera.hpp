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

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace brenta
{

namespace types
{

/**
 * @brief Spherical coordinates
 *
 * This struct represents the spherical coordinates of a point
 * in 3D space.
 */
struct spherical_coordinates
{
    float theta;
    float phi;
    float radius;
    /**
     * @brief Default constructor
     */
    spherical_coordinates() = default;
    /**
     * @brief Constructor
     *
     * Full constructor that initializes the spherical coordinates
     * with the specified values.
     */
    spherical_coordinates(float theta, float phi, float radius)
        : theta(theta), phi(phi), radius(radius)
    {
    }
};

/**
 * @brief Euler angles
 *
 * This struct represents the euler angles of a rigid body
 * in 3D space.
 */
struct euler_angles
{
    float yaw;
    float pitch;
    float roll;
    /**
     * @brief Default constructor
     */
    euler_angles() = default;
    /**
     * @brief Constructor
     *
     * Full constructor that initializes the euler angles
     * with the specified values.
     */
    euler_angles(float yaw, float pitch, float roll)
        : yaw(yaw), pitch(pitch), roll(roll)
    {
    }
};

} // namespace Types

namespace enums
{

enum camera_type
{
    AIRCRAFT,
    SPHERICAL
};

enum projection_type
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

/**
 * @brief Camera movement directions
 *
 * Used to define the direction of the camera movement
 * when reacting to keyboard input.
 */
enum camera_movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

} // namespace Enums

/**
 * @brief The Camera class
 *
 * This class represents a camera in the 3D world. It is used to
 * define the position, orientation and projection of the camera.
 * The class provides a Builder pattern to create a camera object,
 * and it export a default global camera called "camera" that
 * needs to be defined somewhere in the Brenta namespace.
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
 */
class camera
{
  public:
    /**
     * @brief Projection type
     */
    enums::projection_type projection_type;
    /**
     * @brief Camera type
     */
    enums::camera_type camera_type;
    /**
     * @brief Camera position
     */
    glm::vec3 position;
    /**
     * @brief The world up vector
     *
     * This vector defines the up direction in the world space.
     */
    glm::vec3 world_up;
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
    float movement_speed;
    /**
     * @brief Mouse sensitivity
     */
    float mouse_sensitivity;
    /**
     * @brief Zoom level (field of view)
     */
    float zoom;
    /**
     * @brief Spherical coordinates
     *
     * If the camera is of type SPHERICAL, these are the spherical
     * coordinates of the camera. Spherical coordinates are used to
     * represent the position of a point in 3D space, those are theta,
     * phi and radius.
     */
    types::spherical_coordinates spherical_coordinates;
    /**
     * @brief Euler angles
     *
     * If the camera is of type AIRCRAFT, these are the euler angles
     * of the camera. Euler angles are used to represent the orientation
     * of a rigid body in 3D space, those are yaw, pitch and roll.
     */
    types::euler_angles euler_angles;
    /**
     * @brief Front vector
     *
     * This vector represents the front direction of the camera.
     */
    glm::vec3 front;
    /**
     * @brief Up vector
     *
     * This vector represents the up direction of the camera.
     */
    glm::vec3 up;
    /**
     * @brief Right vector
     *
     * This vector represents the right direction of the camera.
     */
    glm::vec3 right;
    /**
     * @brief Is the first mouse movement?
     *
     * This flag is used to check if the mouse movement is the first
     * movement of the camera.
     */
    bool first_mouse = true;
    /**
     * @brief Last x position of the mouse
     */
    float last_x;
    /**
     * @brief Last y position of the mouse
     */
    float last_y;

    /* Constructors */

    /**
     * @brief Default constructor
     *
     * This constructor initializes the camera with default values.
     */
    camera() = default;
    /**
     * @brief Constructor
     *
     * Full constructor that initializes the camera with the specified values.
     */
    camera(enums::camera_type camera_type, enums::projection_type projection_type,
           glm::vec3 position, glm::vec3 world_up, glm::vec3 center,
           float movement_speed, float mouse_sensitivity, float zoom,
           types::spherical_coordinates spherical_coordinates,
           types::euler_angles euler_angles, glm::vec3 front, glm::vec3 up,
           glm::vec3 right);

    /**
     * @brief Builder pattern
     *
     * This class is used to build a camera object. It is used to create
     * a camera object with the desired parameters.
     */
    class builder;

    /* Getters */

    /**
     * @brief Get the camera type
     * @return The camera type
     */
    enums::camera_type get_camera_type();
    /**
     * @brief Get the projection type
     * @return The projection type
     */
    enums::projection_type get_projection_type();
    /**
     * @brief Get the position of the camera
     * @return The position of the camera
     */
    glm::vec3 get_position();
    /**
     * @brief Get the world up vector
     * @return The world up vector
     */
    glm::vec3 get_world_up();
    /**
     * @brief Get the center of the sphere
     * @return The center of the sphere
     */
    glm::vec3 get_center();
    /**
     * @brief Get the movement speed
     * @return The movement speed
     */
    float get_movement_speed();
    /**
     * @brief Get the mouse sensitivity
     * @return The mouse sensitivity
     */
    float get_mouse_sensitivity();
    /**
     * @brief Get the zoom level
     * @return The zoom level
     */
    float get_zoom();
    /**
     * @brief Get the spherical coordinates of the camera
     * @return The spherical coordinates of the camera
     */
    types::spherical_coordinates get_spherical_coordinates();
    /**
     * @brief Get the euler angles of the camera
     * @return The euler angles of the camera
     */
    types::euler_angles get_euler_angles();
    /**
     * @brief Get the view matrix
     * @return The view matrix
     */
    glm::mat4 get_view_matrix();
    /**
     * @brief Get the projection matrix
     * @return The projection matrix
     */
    glm::mat4 get_projection_matrix();
    /**
     * @brief Get the front vector
     * @return The front vector
     */
    glm::vec3 get_front();
    /**
     * @brief Get the up vector
     * @return The up vector
     */
    glm::vec3 get_up();
    /**
     * @brief Get the right vector
     * @return The right vector
     */
    glm::vec3 get_right();
    /**
     * @brief Get the first mouse flag
     * @return boolean flag
     */
    bool get_first_mouse();
    /**
     * @brief Get the last x position of the mouse
     * @return The last x position of the mouse
     */
    float get_last_x();
    /**
     * @brief Get the last y position of the mouse
     * @return The last y position of the mouse
     */
    float get_last_y();

    /* Setters */

    /**
     * @brief Set the camera type
     * @param camera_type The camera type
     */
    void set_camera_type(enums::camera_type camera_type);
    /**
     * @brief Set the projection type
     * @param projection_type The projection type
     */
    void set_projection_type(enums::projection_type projection_type);
    /**
     * @brief Set the world up vector
     * @param worldUp The world up vector
     */
    void set_world_up(glm::vec3 world_up);
    /**
     * @brief Set the center of the sphere
     * @param center The center of the sphere
     */
    void set_center(glm::vec3 center);
    /**
     * @brief Set the movement speed
     * @param movementSpeed The movement speed
     */
    void set_movement_speed(float movement_speed);
    /**
     * @brief Set the mouse sensitivity
     * @param mouseSensitivity The mouse sensitivity
     */
    void set_mouse_sensitivity(float mouse_sensitivity);
    /**
     * @brief Set the zoom level
     * @param zoom The zoom level
     */
    void set_zoom(float zoom);
    /**
     * @brief Set the spherical coordinates of the camera
     * @param sphericalCoordinates The spherical coordinates of the camera
     */
    void
    set_spherical_coordinates(types::spherical_coordinates spherical_coordinates);
    /**
     * @brief Set the euler angles of the camera
     * @param eulerAngles The euler angles of the camera
     */
    void set_euler_angles(types::euler_angles euler_angles);
    /**
     * @brief Set the front vector
     * @param front The front vector
     */
    void set_front(glm::vec3 front);
    /**
     * @brief Set the up vector
     * @param up The up vector
     */
    void set_up(glm::vec3 up);
    /**
     * @brief Set the right vector
     * @param right The right vector
     */
    void set_right(glm::vec3 right);
    /**
     * @brief Set the position of the camera
     * @param position The position of the camera
     */
    void set_position(glm::vec3 position);
    /**
     * @brief Set the first mouse flag
     * @param firstMouse The first mouse flag
     */
    void set_first_mouse(bool first_mouse);
    /**
     * @brief Set the last x position of the mouse
     * @param lastX The last x position of the mouse
     */
    void set_last_x(float last_x);
    /**
     * @brief Set the last y position of the mouse
     * @param lastY The last y position of the mouse
     */
    void set_last_y(float last_y);

    /* Utilities */

    /**
     * @brief Update the camera euler angles
     *
     * This function needs to be called every time the
     * camera euler angles are updated. It calculates
     * the correct front, up and right vectors from
     * the euler angles.
     */
    void update_camera_euler();
    /**
     * @brief Update the camera spherical coordinates
     *
     * This function needs to be called every time the
     * camera spherical coordinates are updated. It the
     * correct position of the camera from the spherical
     * coordinates.
     */
    void spherical_to_cartesian();
};

/**
 * @brief Builder pattern for the Camera class
 */
class camera::builder
{
  private:
    enums::camera_type camera_type = enums::camera_type::AIRCRAFT;
    enums::projection_type projection_type = enums::projection_type::PERSPECTIVE;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    float movement_speed = 2.5f;
    float mouse_sensitivity = 0.1f;
    float zoom = 45.0f;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    types::spherical_coordinates spherical_coordinates = {0.0f, 0.0f, 10.0f};
    types::euler_angles euler_angles = {0.0f, 0.0f, 0.0f};

  public:
    builder &set_camera_type(enums::camera_type camera_type);
    builder &set_projection_type(enums::projection_type projection_type);
    builder &set_position(glm::vec3 position);
    builder &set_world_up(glm::vec3 worldUp);
    builder &set_center(glm::vec3 center);
    builder &set_movement_speed(float movement_speed);
    builder &set_mouse_sensitivity(float mouse_sensitivity);
    builder &set_zoom(float zoom);
    builder &
    set_spherical_coordinates(types::spherical_coordinates spherical_coordinates);
    builder &set_euler_angles(types::euler_angles euler_angles);
    builder &set_front(glm::vec3 front);
    builder &set_up(glm::vec3 up);
    builder &set_right(glm::vec3 right);

    camera build();
};

/**
 * @brief A default camera
 *
 * This is a default camera provided by the engine.
 */
extern camera default_camera;

} // namespace Brenta
