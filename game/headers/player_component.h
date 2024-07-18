#pragma once

#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "engine.h"
#include "model_component.h"

#include <iostream>

using namespace ECS;
using namespace ECS::Types;

/* Player Component */
struct PlayerComponent : Component {};

void InitPlayer() {

    /* Create the player entity */
    auto player_entity = World::NewEntity();


    /* Add the player component */
    auto player_component = std::make_shared<PlayerComponent>();
    World::AddComponent(player_entity, "PlayerComponent", player_component);

    /* Load the shader */
    Shader::NewShader("default_shader",
                      std::filesystem::absolute("game/shaders/shader.vs"),
                      std::filesystem::absolute("game/shaders/shader.fs"));

    /* Load the model */
    Model model(std::filesystem::absolute("assets/models/backpack/backpack.obj"));

    /* Add the model component */
    auto model_component = std::make_shared<ModelComponent>(model, "default_shader");
    World::AddComponent(player_entity, "ModelComponent", model_component);
}

#endif
