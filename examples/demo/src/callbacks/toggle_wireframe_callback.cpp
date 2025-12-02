// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <demo/callbacks/toggle_wireframe_callback.hpp>
#include <demo/resources/wireframe_resource.hpp>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;
using namespace viotecs::types;

void init_toggle_wireframe_callback()
{
  auto toggle_wireframe_callback = []()
  {
    auto wireframe = world::get_resource<WireframeResource>();
    if (wireframe == nullptr)
      return;

    gl::set_poligon_mode(!wireframe->enabled);
    wireframe->enabled = !wireframe->enabled;
  };
  input::add_keyboard_callback(GLFW_KEY_F, toggle_wireframe_callback);
}
