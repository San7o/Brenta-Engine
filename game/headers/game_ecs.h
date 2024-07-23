#pragma once

#ifndef GAME_ECS_H
#define GAME_ECS_H

/* Entities */
#include "entities/player_entity.h"
#include "entities/cube_entity.h"
#include "entities/floor_entity.h"
#include "entities/point_light_entity.h"
#include "entities/directional_light_entity.h"
#include "entities/sphere_entity.h"

/* Components */
#include "components/model_component.h"
#include "components/transform_component.h"
#include "components/point_light_component.h"
#include "components/directional_light_component.h"
#include "components/physics_component.h"
#include "components/sphere_collider_component.h"

/* Systems */
#include "systems/renderer_system.h"
#include "systems/fps_system.h"
#include "systems/debug_text_system.h"
#include "systems/point_lights_system.h"
#include "systems/directional_light_system.h"
#include "systems/physics_system.h"
#include "systems/collisions_system.h"

/* Resources */
#include "resources/wireframe_resource.h"

/* Callbacks */
#include "callbacks/toggle_wireframe_callback.h"
#include "callbacks/close_window_callback.h"
#include "callbacks/camera_mouse_callback.h"
#include "callbacks/play_guitar_callback.h"

#endif
