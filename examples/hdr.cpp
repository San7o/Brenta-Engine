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

/**
 * Just a simple window
 */

#include "ecs.hpp"
#include "engine.hpp"

#include <iostream>
#include <filesystem>

using namespace Brenta;
using namespace Brenta::Types;

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;
const bool isMouseCaptured = false;

using namespace Brenta;
using namespace Brenta::ECS;

struct TransformComponent : Component
{
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    TransformComponent()
        : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
    {
    }
    TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }
};

struct ModelComponent : Component
{
    Model model;
    float shininess;
    Brenta::Types::ShaderName shader;
    bool hasAtlas;
    int atlasSize;
    int atlasIndex;
    int elapsedFrames = 0;

    ModelComponent()
        : model(Model()), shininess(0.0f), shader("default_shader"),
          hasAtlas(false), atlasSize(0), atlasIndex(0)
    {
    }
    ModelComponent(Model model, float shininess, Brenta::Types::ShaderName shader)
        : model(model), shininess(shininess), shader(shader), hasAtlas(false),
          atlasSize(0), atlasIndex(0)
    {
    }
};

struct RendererSystem : System<ModelComponent, TransformComponent>
{
    void run(std::vector<Entity> matches) const override
    {
        if (matches.empty())
            return;

        for (auto match : matches)
        {
            /* Get the model component */
            auto model_component =
                World::EntityToComponent<ModelComponent>(match);

            auto transform_component =
                World::EntityToComponent<TransformComponent>(match);

            auto myModel = model_component->model;
            auto default_shader = model_component->shader;

            Brenta::Types::Translation t = Brenta::Types::Translation();
            t.setView(camera.GetViewMatrix());
            t.setProjection(camera.GetProjectionMatrix());

            t.setModel(glm::mat4(1.0f));
            t.translate(transform_component->position);
            t.rotate(transform_component->rotation);
            t.scale(transform_component->scale);

            t.setShader(default_shader);

            Shader::SetVec3(default_shader, "viewPos", camera.GetPosition());
            Shader::SetFloat(default_shader, "material.shininess",
                             model_component->shininess);

            Shader::SetInt(default_shader, "atlasIndex", 0);
            myModel.Draw(default_shader);
        }
    }
};

REGISTER_SYSTEMS(RendererSystem);

namespace Brenta
{
    Camera camera;
}

int main()
{
    Engine engine = Engine::Builder()
                        .use_screen(true)
                        .use_logger(true)
                        .set_log_level(Brenta::Types::LogLevel::DEBUG)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(isMouseCaptured)
                        .build();

    Camera camera = Camera::Builder()
                        .set_camera_type(Enums::CameraType::AIRCRAFT)
                        .set_projection_type(Enums::ProjectionType::PERSPECTIVE)
                        .set_position(glm::vec3(0.0f, 5.0f, 20.0f))
                        .set_up(glm::vec3(0.0f, 1.0f, 0.0f))
                        .set_eulerAngles(EulerAngles(-90.0f, 0.0f, 0.0f))
                        .build();

    // A square
    float vertices[] = {
        // First Triangle
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,

        // Second Triangle
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };
    VAO vao;
    vao.Init();
    Buffer vbo = Buffer(GL_ARRAY_BUFFER);
    vbo.CopyData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    vao.SetVertexData(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    vao.Unbind();
    vao.Unbind();

    Shader::New("hdr_shader", 
                 GL_VERTEX_SHADER,
                 "examples/hdr.vs",
                 GL_FRAGMENT_SHADER,
                 "examples/hdr.fs");

#ifdef USE_IMGUI
    Brenta::Types::FrameBuffer fb(SCR_WIDTH, SCR_HEIGHT, GL_RGBA16F);
#endif

    ParticleEmitter emitter =
        ParticleEmitter::Builder()
            .set_camera(&camera)
            .set_starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
            .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
            .set_starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
            .set_starting_timeToLive(0.5f)
            .set_num_particles(1000)
            .set_spawn_rate(0.01f)
            .set_scale(1.0f)
            .set_atlas_path(
                std::filesystem::absolute("assets/textures/particle_atlas.png")
                    .string())
            .set_atlas_width(8)
            .set_atlas_height(8)
            .set_atlas_index(5)
            .build();

    // Room ------------------------------------
    auto room_entity = World::NewEntity();
    TransformComponent transform_component;
    World::AddComponent<TransformComponent>(room_entity, transform_component);
    if (Shader::GetId("default_shader") == 0)
    {
        Shader::New("default_shader", GL_VERTEX_SHADER,
                    std::filesystem::absolute("examples/default_shader.vs"),
                    GL_FRAGMENT_SHADER,
                    std::filesystem::absolute("examples/default_shader.fs"));
    }
    Model model(
        std::filesystem::absolute("assets/models/sphere/sphere.obj"));
    auto model_component = ModelComponent(model, 32.0f, "default_shader");
    World::AddComponent<ModelComponent>(room_entity,
                                        std::move(model_component));
    INFO("Room entity created");
    // -----------------------------------------

    Time::Update(Screen::GetTime());
    while (!Screen::isWindowClosed())
    {
        Screen::PollEvents();
        if (Screen::isKeyPressed(GLFW_KEY_ESCAPE))
            Screen::SetClose();

#ifdef USE_IMGUI
        GUI::new_frame(&fb);
        ImGui::Begin("HDR");
        ImGui::End();
#endif
        fb.Bind();

        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();

        // Our scene here

        Shader::Use("hdr_shader");

        emitter.updateParticles(Time::GetDeltaTime());
        emitter.renderParticles();

        Time::Update(Screen::GetTime());
        World::Tick();

        //vao.Bind();
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glCheckError();
        //vao.Unbind();

        fb.Unbind();
#ifdef USE_IMGUI
        GUI::render();
#endif

        Screen::SwapBuffers();
    }

    return 0;
}
