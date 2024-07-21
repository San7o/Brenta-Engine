#pragma once

#ifndef GAME_ECS_H
#define GAME_ECS_H

/* Entities */
#include "entities/player_entity.h"
#include "entities/cube_entity.h"
#include "entities/floor_entity.h"

/* Components */
#include "components/model_component.h"

/* Systems */
#include "systems/renderer_system.h"
#include "systems/fps_system.h"
#include "systems/debug_text_system.h"

/* Resources */
#include "resources/wireframe_resource.h"

/* Callbacks */
#include "callbacks/toggle_wireframe_callback.h"
#include "callbacks/close_window_callback.h"
#include "callbacks/camera_mouse_callback.h"

#endif
