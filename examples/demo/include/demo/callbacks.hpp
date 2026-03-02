// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>
#include <brenta/mouse.hpp>

#include <tenno/memory.hpp>

void init_camera_mouse_callback(tenno::weak_ptr<brenta::Camera> cam,
                                brenta::Mouse *mouse);
void init_close_window_callback();
void init_play_guitar_callback();
void init_toggle_wireframe_callback();
