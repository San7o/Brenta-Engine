// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

// Entities
#include <demo/entities/cube.hpp>
#include <demo/entities/directional_light.hpp>
#include <demo/entities/floor.hpp>
#include <demo/entities/player.hpp>
#include <demo/entities/point_light.hpp>
#include <demo/entities/robot.hpp>
#include <demo/entities/sphere.hpp>

// Components
#include <demo/components/directional_light.hpp>
#include <demo/components/model.hpp>
#include <demo/components/physics.hpp>
#include <demo/components/point_light.hpp>
#include <demo/components/sphere_collider.hpp>
#include <demo/components/transform.hpp>

// Systems
#include <demo/systems/collisions.hpp>
#include <demo/systems/debug_text.hpp>
#include <demo/systems/directional_light.hpp>
#include <demo/systems/fps.hpp>
#include <demo/systems/physics.hpp>
#include <demo/systems/point_lights.hpp>
#include <demo/systems/renderer.hpp>

// Resources
#include <demo/resources/wireframe.hpp>

// Callbacks
#include <demo/callbacks/camera_mouse.hpp>
#include <demo/callbacks/close_window.hpp>
#include <demo/callbacks/play_guitar.hpp>
#include <demo/callbacks/toggle_wireframe.hpp>
