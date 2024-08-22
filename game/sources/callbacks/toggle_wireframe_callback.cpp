#include "engine.hpp"
#include "ecs.hpp"
#include "callbacks/toggle_wireframe_callback.hpp"
#include "resources/wireframe_resource.hpp"

using namespace ECS;
using namespace ECS::Types;

void InitToggleWireframeCallback() {
    auto toggle_wireframe_callback = []() {
        auto wireframe = World::GetResource<WireframeResource>();
        if (wireframe == nullptr) return;

        GL::SetPoligonMode(!wireframe->enabled);
        wireframe->enabled = !wireframe->enabled;
    };
    Input::AddKeyboardCallback(GLFW_KEY_F, toggle_wireframe_callback);
}
