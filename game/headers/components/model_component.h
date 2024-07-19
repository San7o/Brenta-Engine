#pragma once

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "engine.h"

using namespace ECS;

/* Model Component */
struct ModelComponent : Types::Component {
    Model model;
    Types::ShaderName shader;

    ModelComponent(Model model, Types::ShaderName shader)
            : model(model), shader(shader) {}
};

#endif
