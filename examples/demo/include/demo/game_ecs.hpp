// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

/* Entities */
#include <demo/entities/cube_entity.hpp>
#include <demo/entities/directional_light_entity.hpp>
#include <demo/entities/floor_entity.hpp>
#include <demo/entities/player_entity.hpp>
#include <demo/entities/point_light_entity.hpp>
#include <demo/entities/robot_entity.hpp>
#include <demo/entities/sphere_entity.hpp>

/* Components */
#include <demo/components/directional_light_component.hpp>
#include <demo/components/model_component.hpp>
#include <demo/components/physics_component.hpp>
#include <demo/components/point_light_component.hpp>
#include <demo/components/sphere_collider_component.hpp>
#include <demo/components/transform_component.hpp>

/* Systems */
#include <demo/systems/collisions_system.hpp>
#include <demo/systems/debug_text_system.hpp>
#include <demo/systems/directional_light_system.hpp>
#include <demo/systems/fps_system.hpp>
#include <demo/systems/physics_system.hpp>
#include <demo/systems/point_lights_system.hpp>
#include <demo/systems/renderer_system.hpp>

/* Resources */
#include <demo/resources/wireframe_resource.hpp>

/* Callbacks */
#include <demo/callbacks/camera_mouse_callback.hpp>
#include <demo/callbacks/close_window_callback.hpp>
#include <demo/callbacks/play_guitar_callback.hpp>
#include <demo/callbacks/toggle_wireframe_callback.hpp>
