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
