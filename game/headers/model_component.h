#pragma once

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "engine.h"

using namespace ECS;

/* Model Component */
struct ModelComponent : Types::Component {
    Model model;
    Types::ShaderName shader;
    ModelComponent(Entity entity, Model model, Types::ShaderName shader) :
            Types::Component(entity, "ModelComponent"),
            model(model),
            shader(shader) {}
};

#endif
