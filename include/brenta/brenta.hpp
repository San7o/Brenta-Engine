// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

//
// Import everything
//

// opengl
#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/opengl/ubo.hpp>
#include <brenta/renderer/opengl/vao.hpp>

// renderer
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/dir_light.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/point_light.hpp>
#include <brenta/renderer/renderer.hpp>

// Core
#include <brenta/app.hpp>
#include <brenta/asset_manager.hpp>
#include <brenta/sound.hpp>
#include <brenta/color.hpp>
#include <brenta/engine.hpp>
#include <brenta/font.hpp>
#include <brenta/fswatcher.hpp>
#include <brenta/input.hpp>
#include <brenta/key.hpp>
#include <brenta/logger.hpp>
#include <brenta/mouse.hpp>
#include <brenta/node_component.hpp>
#include <brenta/node.hpp>
#include <brenta/scene.hpp>
#include <brenta/script.hpp>
#include <brenta/subsystem.hpp>
#include <brenta/text.hpp>
#include <brenta/time.hpp>
#include <brenta/transform.hpp>
#include <brenta/window.hpp>

#ifndef BRENTA_NO_IMGUI

#include <brenta/gui.hpp>

#endif // BRENTA_NO_IMGUI

// Node components
#include <brenta/node_components/dir_light_node_component.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/node_components/point_light_node_component.hpp>
#include <brenta/node_components/particle_emitter_node_component.hpp>
#include <brenta/node_components/text_node_component.hpp>

#ifndef BRENTA_NO_ECS

#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/camera_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/ecs/components/dir_light_ecs_component.hpp>
#include <brenta/ecs/components/point_light_ecs_component.hpp>
#include <brenta/ecs/components/particle_emitter_ecs_component.hpp>

#include <brenta/ecs/systems/camera_render_system.hpp>
#include <brenta/ecs/systems/model_render_system.hpp>
#include <brenta/ecs/systems/dir_light_render_system.hpp>
#include <brenta/ecs/systems/point_light_render_system.hpp>
#include <brenta/ecs/systems/particle_emitter_system.hpp>

#endif // BRENTA_NO_ECS
