/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include "ecs.hpp"
#include "engine.hpp"

using namespace brenta;
using namespace brenta::ecs;

/* Model Component */
struct ModelComponent : component
{
    model mod;
    float shininess;
    brenta::types::shader_name_t shader;
    bool hasAtlas;
    int atlasSize;
    int atlasIndex;
    int elapsedFrames = 0;

    ModelComponent()
        : mod(model()), shininess(0.0f), shader("default_shader"),
          hasAtlas(false), atlasSize(0), atlasIndex(0)
    {
    }
    ModelComponent(model mod, float shininess,
                   brenta::types::shader_name_t shader, bool hasAtlas = false,
                   int atlasSize = 0, int atlasIndex = 0)
        : mod(mod), shininess(shininess), shader(shader), hasAtlas(hasAtlas),
          atlasSize(atlasSize), atlasIndex(atlasIndex)
    {
    }
};
