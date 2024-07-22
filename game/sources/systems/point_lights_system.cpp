#include "systems/point_lights_system.h"
#include "components/transform_component.h"
#include "components/point_light_component.h"

using namespace ECS;

/* Load the lights on the shaders */
void InitPointLightsSystem() {
    auto light_system = std::make_shared<System>("PointLights", []() {
        auto entities = World::QueryComponents(
                        {"TransformComponent", "PointLightComponent"});
        if (entities.empty()) return;

        int counter = 0;
        for (auto entity : entities) {
            if (counter >= 4) {
                Logger::Log(Types::LogLevel::ERROR,
                            "Only 4 lights are supported");
                break;
            }
            auto transform = static_cast<TransformComponent*>
                    (World::EntityToComponent(entity, "TransformComponent"));

            auto light = static_cast<PointLightComponent*>
                    (World::EntityToComponent(entity, "PointLightComponent"));

            for (auto shader : light->shaders) {
                if (Shader::GetId(shader) == (unsigned int) 0 ) {
                    Logger::Log(Types::LogLevel::ERROR,
                                "Light shader not found with name: " + shader);
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
                Shader::SetInt(shader, "nPointLights", entities.size());
                Shader::SetInt(shader, "material.texture_diffuse1", 0);
                Shader::SetInt(shader, "material.texture_specular1", 1);
                Shader::SetFloat(shader, "material.shininess", 32.0f);
            }
            counter++;
        }

    });

    World::AddSystem(light_system);
}
