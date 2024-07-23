#include "systems/directional_light_system.h"
#include "components/directional_light_component.h"

using namespace ECS;

/* Load the lights on the shaders */
void InitDirectionalLightSystem() {
    auto light_system = std::make_shared<System>("DirectionalLightSystem", []() {
        auto entities = World::QueryComponents({"DirectionalLightComponent"});
        if (entities.empty()) return;

        for (auto entity : entities) {
            auto light = static_cast<DirectionalLightComponent*>
                    (World::EntityToComponent(entity, "DirectionalLightComponent"));

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

    });

    World::AddSystem(light_system);
}
