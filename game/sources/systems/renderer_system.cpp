#include "systems/renderer_system.h"

#include "engine.h"
#include "components/player_component.h"
#include "components/model_component.h"
#include "components/transform_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;
using namespace ECS::Types;

void InitRendererSystem() {

    auto renderer = std::make_shared<System>("RendererSystem", []() {
        /* Get the entities with the model component */
        auto matches = World::QueryComponents({"ModelComponent", "TransformComponent"});
        if (matches.empty()) return;

        for (auto match : matches) {
            /* Get the model component */
            auto model_component = static_cast<ModelComponent*>
                 (World::EntityToComponent(match, "ModelComponent"));

            auto transform_component = static_cast<TransformComponent*>
                 (World::EntityToComponent(match, "TransformComponent"));
            
            auto myModel = model_component->model;
            auto default_shader = model_component->shader;

            Types::Translation t = Types::Translation();
            t.setView(Camera::GetViewMatrix());
            t.setProjection(Camera::GetProjectionMatrix());
            
            t.setModel(glm::mat4(1.0f));
            t.translate(transform_component->position);
            t.rotate(transform_component->rotation);
            t.scale(transform_component->scale);
            
            t.setShader(default_shader);

            Shader::SetVec3(default_shader, "viewPos", Camera::GetPosition());
            Shader::SetFloat(default_shader, "material.shininess", model_component->shininess);

            myModel.Draw(default_shader);
        }
    });

    World::AddSystem(renderer);
    
}
