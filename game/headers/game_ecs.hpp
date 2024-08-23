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

/* Entities */
#include "entities/cube_entity.hpp"
#include "entities/directional_light_entity.hpp"
#include "entities/floor_entity.hpp"
#include "entities/player_entity.hpp"
#include "entities/point_light_entity.hpp"
#include "entities/robot_entity.hpp"
#include "entities/sphere_entity.hpp"

/* Components */
#include "components/directional_light_component.hpp"
#include "components/model_component.hpp"
#include "components/physics_component.hpp"
#include "components/point_light_component.hpp"
#include "components/sphere_collider_component.hpp"
#include "components/transform_component.hpp"

/* Systems */
#include "systems/collisions_system.hpp"
#include "systems/debug_text_system.hpp"
#include "systems/directional_light_system.hpp"
#include "systems/fps_system.hpp"
#include "systems/physics_system.hpp"
#include "systems/point_lights_system.hpp"
#include "systems/renderer_system.hpp"

/* Resources */
#include "resources/wireframe_resource.hpp"

/* Callbacks */
#include "callbacks/camera_mouse_callback.hpp"
#include "callbacks/close_window_callback.hpp"
#include "callbacks/play_guitar_callback.hpp"
#include "callbacks/toggle_wireframe_callback.hpp"
