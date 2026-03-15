// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/renderer/camera.hpp>
#include <tenno/memory.hpp>

void init_cube_entity();
void init_directional_light_entity();
void init_floor_entity();
void init_player_entity();
void init_point_light_entity();
void init_robot_entity();
void init_sphere_entity();
void init_camera_entity(tenno::shared_ptr<brenta::Camera> c,
                        tenno::weak_ptr<brenta::FrameBuffer> fb);
void init_particle_emitter_entity(tenno::shared_ptr<brenta::Camera> c,
                                  tenno::weak_ptr<brenta::FrameBuffer> fb);
