#include "engine.h"
#include "ecs.h"
#include "callbacks/toggle_wireframe_callback.h"
#include "resources/wireframe_resource.h"

using namespace ECS;
using namespace ECS::Types;

void InitToggleWireframeCallback() {
    auto toggle_wireframe_callback = []() {
        auto wireframe = static_cast<WireframeResource*>(World::GetResource<WireframeResource>());
        if (wireframe == nullptr) return;

        GL::SetPoligonMode(!wireframe->enabled);
        wireframe->enabled = !wireframe->enabled;
    };
    Input::AddKeyboardCallback(GLFW_KEY_F, toggle_wireframe_callback);
}
