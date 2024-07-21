#include "engine.h"
#include "callbacks/camera_mouse_callback.h"

using namespace ECS;
using namespace ECS::Types;

void InitCameraMouseCallback() {
    auto camera_mouse_callback = [](double xpos, double ypos) {
        Camera::ProcessMouseMovement(xpos, ypos);
    };
    Input::AddMousePosCallback("CameraCallback", camera_mouse_callback);
}
