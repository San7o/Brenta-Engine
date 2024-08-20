#pragma once

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "engine.h"
#include "ecs.h"

using namespace ECS;

/* Model Component */
struct ModelComponent : Types::Component {
    Model model;
    float shininess;
    Types::ShaderName shader;
    bool hasAtlas;
    int atlasSize;
    int atlasIndex;
    int elapsedFrames = 0;

    ModelComponent(Model model, float shininess, Types::ShaderName shader,
                    bool hasAtlas = false, int atlasSize = 0,
                    int atlasIndex = 0)
            : model(model), shininess(shininess), shader(shader),
              hasAtlas(hasAtlas), atlasSize(atlasSize),
              atlasIndex(atlasIndex) {}
};

#endif
