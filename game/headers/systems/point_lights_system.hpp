#pragma once

#include "ecs.hpp"
#include "systems/point_lights_system.hpp"
#include "components/transform_component.hpp"
#include "components/point_light_component.hpp"

#include <vector>

using namespace Brenta::ECS;

/* Load the lights on the shaders */
struct PointLightsSystem : System<TransformComponent, PointLightComponent> {
    void run(std::vector<Entity> entities) const override {
        if (entities.empty()) return;

        int counter = 0;
        for (auto entity : entities) {
            if (counter >= 4) {
                ERROR("Only 4 lights are supported");
                break;
            }
            auto transform = World::EntityToComponent<TransformComponent>(entity);

            auto light = World::EntityToComponent<PointLightComponent>(entity);

            for (auto shader : light->shaders) {
                if (Shader::GetId(shader) == (unsigned int) 0 ) {
                    ERROR("Light shader not found with name: ", shader);
                    continue;
                }
                Shader::Use(shader);

                /* Set the light properties */
                std::string lightn = "pointLights[" +
                                      std::to_string(counter) + "]";
                Shader::SetVec3(shader, (lightn+".ambient").c_str(), light->ambient);
                Shader::SetVec3(shader, (lightn+".diffuse").c_str(), light->diffuse);
                Shader::SetVec3(shader, (lightn+".specular").c_str(), light->specular);
                Shader::SetFloat(shader, (lightn+".constant").c_str(), light->constant);
                Shader::SetFloat(shader, (lightn+".linear").c_str(), light->linear);
                Shader::SetFloat(shader, (lightn+".quadratic").c_str(), light->quadratic);
                Shader::SetFloat(shader, (lightn+".strength").c_str(), light->strength);
                Shader::SetVec3(shader, (lightn+".position").c_str(), transform->position);
                Shader::SetFloat(shader, (lightn+".point_strength").c_str(), light->strength);
                Shader::SetInt(shader, "nPointLights", entities.size());
            }
            counter++;
        }

    }
};
