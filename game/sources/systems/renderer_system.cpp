#include "systems/renderer_system.h"

#include "engine.h"
#include "components/player_component.h"
#include "components/model_component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ECS;
using namespace ECS::Types;

void InitRendererSystem() {

    auto renderer = std::make_shared<System>("Renderer", []() {
        /* Get the entities with the model component */
        auto matches = World::QueryComponents({"ModelComponent"});
        if (matches.empty()) return;

        for (auto match : matches) {
            /* Get the model component */
            auto model_component = static_cast<ModelComponent*>
                 (World::EntityToComponent(match, "ModelComponent"));
            
            auto myModel = model_component->model;
            auto default_shader = model_component->shader;

            Types::Translation t = Types::Translation();
            t.setProjection(45.0f, 0.1f, 100.0f);
            t.setModel(glm::mat4(1.0f));
            t.translate(glm::vec3(0.0f, 0.0f, -10.0f));
            t.setShader(default_shader);

            myModel.Draw(default_shader);
        }
    });

    World::AddSystem(renderer);
    
}
