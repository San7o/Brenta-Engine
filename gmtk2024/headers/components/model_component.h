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
    bool hasAtlas;
    int atlasSize;
    int atlasIndex;
    int elapsedFrames;
    int isAnimated;
    int animationLength;
    int animationSpeed;
    int startingIndex;

    ModelComponent(Model model, float shininess, Types::ShaderName shader,
                    bool hasAtlas = false, int atlasSize = 0,
                    int atlasIndex = 0, int isAnimated = true,
                    int animationLength = 0, int animationSpeed = 0,
                    int startingIndex = 0)
            : model(model), shininess(shininess), shader(shader),
              hasAtlas(hasAtlas), atlasSize(atlasSize),
              atlasIndex(atlasIndex), isAnimated(isAnimated),
              animationLength(animationLength), animationSpeed(animationSpeed),
              startingIndex(startingIndex) {}
};

#endif
