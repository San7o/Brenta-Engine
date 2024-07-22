#pragma once

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "engine.h"

using namespace ECS;

/* Model Component */
struct ModelComponent : Types::Component {
    Model model;
    float shininess;
    Types::ShaderName shader;

    ModelComponent(Model model, float shininess, Types::ShaderName shader)
            : model(model), shininess(shininess), shader(shader) {}
};

#endif
