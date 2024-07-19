#pragma once

#ifndef WIREFRAME_RESOURCE_H
#define WIREFRAME_RESOURCE_H

#include "resource.h"

using namespace ECS;

struct WireframeResource : Types::Resource {
    bool enabled;
    WireframeResource(bool e) : enabled(e) {}
};

void InitWireframeResource();

#endif
