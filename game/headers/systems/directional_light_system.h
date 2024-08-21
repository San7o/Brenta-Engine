#pragma once

#include "ecs.h"
#include "systems/directional_light_system.h"
#include "components/directional_light_component.h"

#include <vector>

using namespace ECS;

struct DirectionalLightSystem : System<DirectionalLightComponent> {

    void run(std::vector<Entity> entities) const override {

        if (entities.empty()) return;

        for (auto entity : entities) {
            auto light = World::EntityToComponent<DirectionalLightComponent>(entity);

            for (auto shader : light->shaders) {
                if (Shader::GetId(shader) == (unsigned int) 0 ) {
                    Logger::Log(Types::LogLevel::ERROR,
                                "Light shader not found with name: " + shader);
                    continue;
                }
                Shader::Use(shader);

                /* Set the light properties */
                Shader::SetVec3(shader, "dirLight.direction", light->direction);
                Shader::SetVec3(shader, "dirLight.ambient", light->ambient);
                Shader::SetVec3(shader, "dirLight.diffuse", light->diffuse);
                Shader::SetVec3(shader, "dirLight.specular", light->specular);
                Shader::SetFloat(shader, "dirLight.dir_strength", light->strength);
                Shader::SetBool(shader, "useDirLight", true);
            }
        }

    }
};
