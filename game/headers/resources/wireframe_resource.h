#pragma once

#include "resource.h"

using namespace ECS;

struct WireframeResource : Resource {
    bool enabled;
    WireframeResource(bool e) : enabled(e) {}
};

void InitWireframeResource();
