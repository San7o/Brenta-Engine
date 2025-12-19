// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
 * This struct represents the spherical coordinates of a point in 3D
 * space.
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
   * Full constructor that initializes the spherical coordinates with
   * the specified values.
   */
  spherical_coordinates(float theta, float phi, float radius)
      : theta(theta), phi(phi), radius(radius)
  {
  }
};

/**
 * @brief Euler angles
 *
 * This struct represents the euler angles of a rigid body in 3D
 * space.
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
   * Full constructor that initializes the euler angles with the
   * specified values.
   */
  euler_angles(float yaw, float pitch, float roll)
      : yaw(yaw), pitch(pitch), roll(roll)
  {
  }
};

} // namespace types

/**
 * @brief The Camera class
 *
 * This class represents a camera in the 3D world. It is used to
 * define the position, orientation and projection of the camera.  The
 * class provides a builder pattern to create a camera object, and it
 * export a default global camera called "camera" that needs to be
 * defined somewhere in the Brenta namespace.
 *
 * ## Camera type
 *
 * The camera can be of two types: aircraft or spherical. The aircraft
 * type is used to represent a camera that moves in the world space,
 * while the spherical type is used to represent a camera that rotates
 * around a center point. They use respectively euler angles and
 * spherical coordinates to represent the position of the camera.
 *
 * ## Projection
 *
 * The camera can have two types of projection: perspective or
 * prthographic.
 */
class camera
{
public:
  
  enum class camera_type
  {
    aircraft,
    spherical
  };

  enum class projection_type
  {
    perspective,
    orthographic
  };

  /**
   * @brief Camera movement directions
   *
   * Used to store the direction of the camera movement when reacting
   * to keyboard input.
   */
  enum class camera_movement
  {
    forward,
    backward,
    left,
    right
  };
  
  projection_type proj_type;
  camera_type cam_type;
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
   * If the camera is of type AIRCRAFT, these are the euler angles of
   * the camera. Euler angles are used to represent the orientation of
   * a rigid body in 3D space, those are yaw, pitch and roll.
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

  class config;
  
  // Constructors

  /**
   * @brief Default constructor
   *
   * This constructor initializes the camera with default values.
   */
  camera() = default;
  /**
   * @brief Constructor
   *
   * Full constructor that initializes the camera with the specified
   * values.
   */
  camera(config conf);

  /**
   * @brief Builder pattern
   *
   * This class is used to build a camera object. It is used to create
   * a camera object with the desired parameters.
   */
  class builder;

  // Getters

  camera::camera_type get_camera_type();
  camera::projection_type get_projection_type();
  glm::vec3 get_position();
  glm::vec3 get_world_up();
  glm::vec3 get_center();
  float get_movement_speed();
  float get_mouse_sensitivity();
  float get_zoom();
  types::spherical_coordinates get_spherical_coordinates();
  types::euler_angles get_euler_angles();
  glm::mat4 get_view_matrix();
  glm::mat4 get_projection_matrix(int window_width, int window_height);
  glm::vec3 get_front();
  glm::vec3 get_up();
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

  // Setters

  void set_camera_type(camera::camera_type camera_type);
  void set_projection_type(camera::projection_type projection_type);
  void set_world_up(glm::vec3 world_up);
  void set_center(glm::vec3 center);
  void set_movement_speed(float movement_speed);
  void set_mouse_sensitivity(float mouse_sensitivity);
  void set_zoom(float zoom);
  void
  set_spherical_coordinates(types::spherical_coordinates spherical_coordinates);
  void set_euler_angles(types::euler_angles euler_angles);
  void set_front(glm::vec3 front);
  void set_up(glm::vec3 up);
  void set_right(glm::vec3 right);
  void set_position(glm::vec3 position);
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

  // Utilities

  /**
   * @brief Update the camera euler angles
   *
   * This function needs to be called every time the camera euler
   * angles are updated. It calculates the correct front, up and right
   * vectors from the euler angles.
   */
  void update_camera_euler();
  /**
   * @brief Update the camera spherical coordinates
   *
   * This function needs to be called every time the camera spherical
   * coordinates are updated. It sets the correct position of the
   * camera from the spherical coordinates.
   */
  void spherical_to_cartesian();
};

struct camera::config
{
  camera_type cam_type;
  projection_type proj_type;
  glm::vec3 position;
  glm::vec3 world_up;
  glm::vec3 center;
  float movement_speed;
  float mouse_sensitivity;
  float zoom;
  types::spherical_coordinates spherical_coordinates;
  types::euler_angles euler_angles;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
};
  
/**
 * @brief Builder pattern for the Camera class
 */
class camera::builder
{
private:

  camera::config conf = {
    camera::camera_type::aircraft,
    camera::projection_type::perspective,
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    2.5f,
    0.1f,
    45.0f,
    {0.0f, 0.0f, 10.0f},
    {0.0f, 0.0f, 0.0f},
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
  };

public:
  
  builder &camera_type(camera::camera_type camera_type);
  builder &projection_type(camera::projection_type projection_type);
  builder &position(glm::vec3 position);
  builder &world_up(glm::vec3 worldUp);
  builder &center(glm::vec3 center);
  builder &movement_speed(float movement_speed);
  builder &mouse_sensitivity(float mouse_sensitivity);
  builder &zoom(float zoom);
  builder &
  spherical_coordinates(types::spherical_coordinates spherical_coordinates);
  builder &euler_angles(types::euler_angles euler_angles);
  builder &front(glm::vec3 front);
  builder &up(glm::vec3 up);
  builder &right(glm::vec3 right);

  camera build();
};

} // namespace brenta
