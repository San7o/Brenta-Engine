#include "engine.hpp"
#include "callbacks/camera_mouse_callback.hpp"

using namespace ECS;
using namespace ECS::Types;

void InitCameraMouseCallback() {

    auto camera_mouse_callback = [](double xpos, double ypos) {

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
 
            camera.sphericalCoordinates.theta += yoffset * camera.MouseSensitivity;
            camera.sphericalCoordinates.phi   += xoffset * camera.MouseSensitivity;

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
            glm::vec3 fixed_center = glm::vec3(camera.center.x, camera.Position.y, camera.center.z);
            glm::vec3 front = glm::normalize(camera.Position - fixed_center);    // Versor
            glm::vec3 right = glm::normalize(glm::cross(front, camera.WorldUp)); // Versor

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
        else {
            camera.firstMouse = true;
        }
    };
    Input::AddMousePosCallback("CameraCallback", camera_mouse_callback);
}
