// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

//
// Import everything
//

// renderer
#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/opengl/vao.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/scene.hpp>
#include <brenta/renderer/transform.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/light.hpp>

// Core
#include <brenta/audio.hpp>
#include <brenta/mouse.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/time.hpp>
#include <brenta/subsystem.hpp>
#include <brenta/ecs.hpp>
#include <brenta/engine.hpp>

// Gui
#include <brenta/gui.hpp>
#include <brenta/font.hpp>
#include <brenta/text.hpp>
