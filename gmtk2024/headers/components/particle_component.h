#pragma once

#include "engine.h"
#include <memory>

using namespace ECS;

/* Particle Component */
struct ParticleComponent : Types::Component {
    std::shared_ptr<ParticleEmitter> emitter;

    ParticleComponent(std::shared_ptr<ParticleEmitter> emitter) {
        this->emitter = emitter;
    }
};
