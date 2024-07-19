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

            /* Setup the shader */
            Shader::Use(default_shader);

            glm::mat4 view = glm::mat4(1.0f); /* Camera position */
            glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                           (float)800/(float)600, 0.1f, 100.0f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));

            Shader::SetMat4(default_shader, "view", view);
            Shader::SetMat4(default_shader, "projection", projection);
            Shader::SetMat4(default_shader, "model", model);
            
            myModel.Draw(default_shader);
        }
    });
    World::AddSystem(renderer);
    
}
