#pragma once

#include "components/model_component.hpp"
#include "components/player_component.hpp"
#include "components/transform_component.hpp"
#include "ecs.hpp"
#include "engine.hpp"
#include "systems/renderer_system.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define ANIMATION_SPEED 24

using namespace Brenta::ECS;

struct RendererSystem : System<ModelComponent, TransformComponent>
{
    void run (std::vector<Entity> matches) const override
    {
        if (matches.empty ())
            return;

        for (auto match : matches)
        {
            /* Get the model component */
            auto model_component =
                World::EntityToComponent<ModelComponent> (match);

            auto transform_component =
                World::EntityToComponent<TransformComponent> (match);

            auto myModel = model_component->model;
            auto default_shader = model_component->shader;

            Brenta::Types::Translation t = Brenta::Types::Translation ();
            t.setView (camera.GetViewMatrix ());
            t.setProjection (camera.GetProjectionMatrix ());

            t.setModel (glm::mat4 (1.0f));
            t.translate (transform_component->position);
            t.rotate (transform_component->rotation);
            t.scale (transform_component->scale);

            t.setShader (default_shader);

            Shader::SetVec3 (default_shader, "viewPos", camera.GetPosition ());
            Shader::SetFloat (default_shader, "material.shininess",
                              model_component->shininess);

            /* Animation control */
            if (model_component->hasAtlas)
            {
                if (model_component->elapsedFrames > ANIMATION_SPEED)
                {
                    model_component->elapsedFrames = 0;
                    model_component->atlasIndex++;
                    if (model_component->atlasIndex
                        >= model_component->atlasSize)
                    {
                        model_component->atlasIndex = 0;
                    }
                }
                else
                {
                    model_component->elapsedFrames++;
                }
                Shader::SetInt (default_shader, "atlasSize",
                                model_component->atlasSize);
                Shader::SetInt (default_shader, "atlasIndex",
                                model_component->atlasIndex);
            }
            else
            {
                Shader::SetInt (default_shader, "atlasIndex", 0);
            }

            myModel.Draw (default_shader);
        }
    }
};
