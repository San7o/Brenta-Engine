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

#include "buffer.hpp"
#include "camera.hpp"
#include "engine_audio.hpp"
#include "engine_input.hpp"
#include "engine_logger.hpp"
#include "engine_time.hpp"
#include "gl_helper.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "particles.hpp"
#include "screen.hpp"
#include "shader.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "translation.hpp"
#include "vao.hpp"

namespace Brenta
{

/**
 * @brief Engine setup
 *
 * This class is used to setup every subsystem of the engine. You can
 * use the builder class to set the parameters of the engine: you can set
 * which subsystem to use and configure them.
 *
 * It automatically destroys the subsystems when the engine is destroyed.
 */
class Engine
{
  public:
    bool uses_screen;
    bool uses_audio;
    bool uses_input;
    bool uses_logger;
    bool uses_text;
    bool uses_ecs;
    int screen_width;
    int screen_height;
    bool screen_is_mouse_captured;
    const char *screen_title;
    bool screen_msaa;
    bool screen_vsync;
    Types::LogLevel log_level;
    std::string log_file;
    std::string text_font;
    int text_size;
    bool gl_blending;
    bool gl_cull_face;
    bool gl_multisample;
    bool gl_depth_test;

    Engine(bool uses_screen, bool uses_audio, bool uses_input, bool uses_logger,
           bool uses_text, bool uses_ecs, int screen_width, int screen_height,
           bool screen_is_mouse_captured, bool screen_msaa, bool screen_vsync,
           const char *screen_title, Types::LogLevel log_level,
           std::string log_file, std::string text_font, int text_size,
           bool gl_blending, bool gl_cull_face, bool gl_multisample,
           bool gl_depth_test);
    ~Engine();

    class Builder;
};

/**
 * @brief Engine builder
 *
 * This class is used to build the engine. You can set the parameters of the
 * engine using the methods of this class and then call the build method to
 * create the engine.
 */
class Engine::Builder
{
  public:
    bool uses_screen = false;
    bool uses_audio = false;
    bool uses_input = false;
    bool uses_logger = false;
    bool uses_text = false;
    bool uses_ecs = false;
    int screen_width = 1280;
    int screen_height = 720;
    bool screen_is_mouse_captured = false;
    bool screen_msaa = false;
    bool screen_vsync = false;
    const char *screen_title = "";
    Types::LogLevel log_level = Types::LogLevel::WARNING;
    std::string log_file = "./logs/log.txt";
    std::string text_font = "arial.ttf";
    int text_size = 48;
    bool gl_blending = true;
    bool gl_cull_face = true;
    bool gl_multisample = true;
    bool gl_depth_test = true;

    Builder &use_screen(bool uses_screen);
    Builder &use_audio(bool uses_audio);
    Builder &use_input(bool uses_input);
    Builder &use_logger(bool uses_logger);
    Builder &use_text(bool uses_text);
    Builder &use_ecs(bool uses_ecs);
    Builder &set_screen_width(int screen_width);
    Builder &set_screen_height(int screen_height);
    Builder &set_screen_is_mouse_captured(bool screen_is_mouse_captured);
    Builder &set_screen_title(const char *screen_title);
    Builder &set_screen_msaa(bool screen_msaa);
    Builder &set_screen_vsync(bool screen_vsync);
    Builder &set_log_level(Types::LogLevel log_level);
    Builder &set_log_file(std::string log_file);
    Builder &set_text_font(std::string text_font);
    Builder &set_text_size(int text_size);
    Builder &set_gl_blending(bool gl_blending);
    Builder &set_gl_cull_face(bool gl_cull_face);
    Builder &set_gl_multisample(bool gl_multisample);
    Builder &set_gl_depth_test(bool gl_depth_test);

    Engine build();
};

} // namespace Brenta

// Doxygen Documnetation:

// cmake-format: off
/*! \mainpage Home
 *
 * \section intro_sec Introduction
 *
 * Hi and welcome to the **Brenta Engine documentation**! Brenta Engine is a simple 3D
 * engine writter in modern  C++/OpenGL using the Entity Component System architecture.
 * The engine was created by Giovanni Santini in the summer of 2024, the name is inspired
 * by the Brenta Dolimites in the Italian Alps.
 *
 * \image html adamello-brenta.jpg
 *
 * \section submodules The Submodules
 *
 * The Engine is composed of many different submodules, each one with its own purpose.
 * The submodules are mostly independent from each other so you can use any of the submodules
 * you are interested into. I said mostly because all the submodules that work in a graphical
 * environment need the opengl context from the Screen.
 *
 * The main submodules are:
 * - **Brenta::Screen**: manages the window and the OpenGL context.
 * - **Brenta::Audio**: everytging audio.
 * - **Brenta::Input**: manages the screen input using callbacks.
 * - **Brenta::Utils::Logger**: logging manager.
 * - **Brenta::Text**: text rendering.
 * - **Brenta::ECS::World**: the Entity Component System submodule.
 * - **Brenta::Engine**: manages the setup of the engine.
 *
 * In addition, the engine has multiple classes to provide more functionalities.
 * Those are mostly wrappers around opengl primitives or handy classes providing
 * utilities. Classes with many variables also provide a Builder to nicely
 * iniitalize them.
 *
 * The classes are:
 * - **Brenta::GL**: provides some useful OpenGL functions.
 * - **Brenta::Mesh**: a 3D openGL mesh.
 * - **Brenta::Model**: a 3D openGL model.
 * - **Brenta::ParticleEmitter**: create and customize particles.
 * - **Brenta::Shader**: manages the shaders.
 * - **Brenta::Texture**: manages the textures.
 * - **Brenta::Types::Translation**: manages the translations.
 * - **Brenta::Types::VAO**: wrapper around the Vertex Array Objects.
 * - **Brenta::Types::Buffer**: wrapper around the Buffers.
 * - **Brenta::Time**: manages the time.
 * - **Brenta::Camera**: manages the camera.
 *
 * \section graphics Graphics
 *
 * The engine uses OpenGL for the graphics and provides classes to interact with opengl.
 *
 * The engine natively provides the following features:
 * - **Color Blending**
 * - **Cull Face**
 * - **Multisample**
 * - **Depth Test**
 * - **MSAA**
 * - **Vsync**
 * - **obj model loading**
 * - **Text rendering**
 * - **Lighting**: Ambient, Diffuse and Specular lighting.
 * - **Texture atlas**: texture atlas support.
 * - **GPU Particles**: particle system in the GPU.
 *
 * Although the engine currently implements only basic graphics features, it provides 
 * all the building blocks to create more complex graphics. Effort will be put into
 * implementing various graphics techniques, if you want to contribute check out the
 * \ref Contributing page. <a href="https://github.com/San7o/Brenta-Engine/issues/10">This</a> 
 * issue tracks future features to implement.
 *
 * In the next section we are going to see how to build the engine.
 *
 * \section license License
 *
 * The engine is released under the MIT license. You can find the license in the repository.
 *
 * \page Building
 *
 * The engine is written entirely in C++ and uses CMake as the build system. In this section
 * we will guide you through the process of building the engine.
 *
 * \section supported_platforms Supported Platforms
 *
 * Currently, the only officially supported platform is **Linux**. The developer is aiming
 * to release the engine on Windows and MacOS in the future. If you want to contribute
 * to this issue, feel free to do so and check out the \ref Contributing page.
 * This guide will go through the process on a linux system.
 * 
 * \section dependencies Dependencies
 *
 * The engine has the following dependencies:
 * - **gcc** compiler
 * - **CMake**
 * - **GLFW**: window and input handling
 * - **librefreetype**: text rendering
 * - **libassimp**: model loading
 * - **SDL3**: audio driver
 *
 * In addition, the following programs are optional:
 * - **Doxygen**: to generate the documentation
 * - **cmake-format**: code formatter
 *
 * You may or may not have the libraries available in your packet manger, if not you will
 * need to build them from source. This guide will show you an example of building libassimp.
 *
 * \subsection nix Nix
 *
 * If you are using Nix, a development shell is provided in the repository. You can enter
 * the shell by running the following command:
 * ```
 * nix develop
 * ```
 * You should now have all the dependencies installed and ready to build the engine.
 *
 * \subsection libassimo Building libassimp
 *
 * You need <a href="https://github.com/assimp/assimp">libassimp</a> to load 3D models. To
 * build it from source, first clone the repository:
 * ```
 * git clone https://github.com/assimp/assimp.git
 * ```
 *
 * You can build the library with the following commands:
 * ```
 * cd assimp/
 * cmake CMakeLists.txt -DASSIMP_WARNINGS_AS_ERRORS=off -DASSIMP_INSTALL=off
 * make -j4
 * ```
 * The library will be generated in `lib/`. If you don't install the library system-wide (ASSIMP_INSTALL=off),
 * you need to specify the path in an environment variable to run the application, like so:
 * ```bash
 * LD_LIBRARY_PATH=${PWD}/lib/:${LD_LIBRARY_PATH} ./build/main.out
 * ```
 *
 * Likewise, you can build the other dependencies from source.
 * 
 * \section compiling Compiling the Game
 *
 * To compile the demo game, run the following commands:
 * ```
 * cmake -Bbuild
 * cmake --build build
 * ```
 * The binaries will be generated in the `build/` directory.
 *
 * Run the game with:
 * ```
 * ./build/main
 * ```
 *
 * \section documentation Building documentation
 * 
 * You can build the documentation with `doxygen` (you need to have doxygen installed
 * in your system):
 * ```bash
 * doxygen doxygen.conf
 * ```
 * The index will be located in `docs/html/index.html`.
 * 
 * \section format Format code
 * To execute the formatter, run:
 * ```bash
 * make format
 * ```
 * 
 * \section tests Tests
 * 
 * To build tests, run:
 * ```bash
 * cmake -Bbuild -DBUILD_TESTS=ON
 * cmake --build build -j 4
 * ./buid/unit
 * ```
 * 
 * \section examples Examples
 * 
 * There is an `examples` directory, you can run an exmple with the following command:
 * ```bash
 * cmake -Bbuild -DBUILD_EXAMPLES=ON
 * cmake --build build -j 4 --target load_model
 * ./build/load_model
 * ```
 * Change target to the example you want to compile
 * 
 * \page Directory Structure
 * This section will guide you through the directory structure of the engine.
 *
 * \section directories Directories
 * The engine is divided into the following directories:
 *
 * - **engine**: contains the source code of the engine
 * - **ecs**: contains all the files related to the Entity Component System
 * - **utils**: contains utility classes like the Logger, those classes are ussed both by
 *              the engine and the ECS
 * - **tests**: contains the tests for the engine
 * - **examples**: contains some examples on features of the engine, to build them follow the instructions in the \ref Building page
 * - **include**: contains third party header files
 * - **lib**: contains the third party libraries
 * - **game**: source code of a demo game
 * - **assets**: directory for assets like models, textures, etc.
 * 
 * \section files Files
 * - **clang-format**: the formatting configuration file for clang-format
 * - **doxygen.conf**: the configuration file for doxygen
 * - **CMakeLists.txt**: the main CMake file
 * - **LICENSE**: the license file
 *
 * \section namespaces Namespaces
 * The engine uses the `Brenta` namespace. There are also some subnamespaces:
 * - **Brenta::Types**: contains the types used in the engine
 * - **Brenta::ECS**: contains the types used in the ECS
 * - **Brenta::Utils**: contains the utility classes (the logger)
 *
 * \page ECS
 * Brenta Engine features an Entity Component System architecture. The ECS is a
 * design pattern that allows you to structure your code in a way that is more modular and
 * scalable.
 *
 * The ECS is composed of three main parts:
 * - **Entity**: an entity is an object in the game, it is just an ID
 * - **Component**: a component is a piece of data that is attached to an entity
 * - **System**: a system is a piece of code that operates on entities with specific components. They get autocamically executed at each game tick.
 * - **Resources**: resources are shared global data that can be accessed by any system
 * 
 * With those building blocks, you can create any game or application. The ECS is optional, you can use the engine without it.
 *
 * \subsection header Header
 * Everything you need to use the ECS is in the `ecs` directory. To use the ECS, include the `ecs.hpp` header:
 * ```cpp
 * #include "ecs.hpp"
 * using namespace Brenta;
 * ```
 *
 * \section world The World
 *
 * Everything that exists in the ECS is contained in the `World` class. The World
 * is a singleton accessible from anywhere in the code and It will be your entry
 * point to the ECS. You can create entities, assign components to them, and query
 * entities with specific components. Let's take a look on how to use the World API.
 *
 * \section entity Creating new Entities
 * Creating an entity is very straightforward, you can ask the world to create one for you:
 * ```cpp
 * Entity entity = World::NewEntity();
 * ```
 * This will return a new entity with a unique ID or -1 if the entity could not be created.
 *
 * \section component Assigning Components to Entities
 *
 * To assign a component to an entity, first you need to create a component.
 * Components are just structs with data that extend the `Component` class. Note that you
 * need to implement a default empty constructor for your component.
 *
 * For example:
 * ```cpp
 * struct PositionComponent : public Component {
 *     float x, y, z;
 *     PositionComponent(); // Required
 *     PositionComponent(float x, float y, float z); // Optional
 * }
 * ```
 *
 * You can find many more examples in `examples/components.cpp`.
 *
 * Once you have created a component, you can assign it to an entity:
 * ```cpp
 * PositionComponent position(0.0f, 0.0f, 0.0f);
 * World::AddComponent<PositionComponent>(entity, position);
 * ```
 *
 * \section system Creating Systems
 *
 * Systems are classes that extend the `System` class. Systems can query entities
 * with specific components and operate on them. They have a run method that will
 * be called at each tick. Systems can define which components
 * to query by specifying them in the template arguments (see the example below),
 * they will receive a vector of entities that match the query.
 * 
 * **Note** that if your System does not need to query any component, you need to pass
 * `None` as the template argument.
 * 
 * You can create a system like so:
 *
 * ```cpp
 *
 * struct RendererSystem : System<ModelComponent, TransformComponent> {
 *   // you need to implement the run method
 *   void run(std::vector<Entity> matches) const override {
 *     if (matches.empty()) return;
 *
 *     for (auto match : matches) {
 *         // get the components associated to an entity
 *         auto model_component = World::EntityToComponent<ModelComponent>(match);
 *         
 *         // ...
 *
 *         myModel.Draw(default_shader);
 *     }
 *   }
 * };
 * ```
 * Note how we used `World::EntityToComponent` to get the components associated with an entity.
 * This is a common pattern you will use in your systems.
 *
 * After you defined your system, you need to register it. This can be done throught
 * a macro:
 * ```cpp
 * REGISTER_SYSTEMS(RendererSystem);
 * ```
 * **Note**: You can register as many systems as you want but passing the systems as a comma
 * separated list to the macro, but you have to use `REGISTER_SYSTEM` once
 * and only once in your code, this is because systems are registered in a global type.
 *
 * \section resources Resources
 *
 * Resources are data, like components, but they are not associated with entities. Things like
 * the day in the game, the number of enemies, etc should be resources.
 *
 * You can create a resource like so:
 * ```cpp
 * struct DayResource : public Resource {
 *    int day;
 *    DayResource() : day(0) {}
 *    DayResource(int day) : day(day) {}
 * };
 * ```
 *
 * You can register a resource like so:
 * ```cpp
 * World::AddResource<WireframeResource>(WireframeResource(false));
 * ```
 *
 *
 * \page Engine
 * In this section we will see how to use various features of the engine.
 *
 * \section engine Engine Class
 * To initialize any subsystem, you can use the Engine class. This is not mandatory, you can
 * initialize and destroy the subsystems manually, but the Engine class provides a nice
 * way to do it.
 *
 * To create an engine, you can use the Builder class:
 * ```cpp
 * Brenta::Engine engine = Brenta::Engine::Builder()
 *     .use_screen(true)
 *     .use_audio(true)
 *     .use_input(true)
 *     .use_logger(true)
 *     .use_text(true)
 *     .use_ecs(true)
 *     .set_screen_width(1280)
 *     .set_screen_height(720)
 *     // ...
 *     .build();
 * ```
 * You can read every option in the `Brenta::Engine::Builder` class.
 * Let's go through some subsystems.
 *
 * \section input Input
 *
 * You can register callback functions for the input. Those functions are called
 * when the specified `key` is pressed, or the mouse is moved, depending on the
 * callback you register.
 *
 * ```cpp
 * auto toggle_wireframe_callback = []() {
 * 
 *     auto wireframe = World::GetResource("WireframeResource");
 *     if (wireframe == nullptr) return;
 * 
 *     GL::SetPoligonMode(!wireframe->enabled);
 *     wireframe->enabled = !wireframe->enabled;
 * };
 * 
 * Input::AddkeyboardCallback(GLFW_KEY_F, toggle_wireframe_callback);
 * ```
 * In this example we register a keyboard callback that toggles the wireframe mode
 * when the `F` key is pressed. You can use `Brenta::Input::AddMousePosCallback`
 * to register a mouse callback, this ill be called with the x and y position of the mouse.
 *
 * You can also remove the callbacks with `Brenta::Input::RemoveKeyboardCallback` and
 * `Brenta::Input::RemoveMousePosCallback`.
 *
 * \section audio Audio
 *
 * The audio subsystem is very simple: there are audio streams and audio files,
 * you can play an audio file on a stream (not more) and stop it, so you need
 * to have multiple streams if you want to play multiple audio files at the same time.
 *
 * You can load an audio file like so:
 * ```cpp
 * Audio::LoadAudio("guitar", "assets/audio/guitar.wav");
 * ```
 * We are identifying this audio file with the name `guitar`.
 *
 * You can create a stream with the name "music" like so:
 * ```cpp
 * Audio::CreateStream("music");
 * ```
 *
 * And finally play the "guitar" audio like so:
 * ```cpp
 * Audio::PlayAudio("guitar", "music");
 * ```
 * The subsystem will provide you a default stream named "default" if you don't want to
 * create a stream.
 *
 * You can Pause and Resume streams with `Brenta::Audio::PauseStream` and
 * `Brenta::Audio::ResumeStream`, set the volume and stop it. You can find
 * the API in `Brenta::Audio`.
 *
 * \section particles Particles
 *
 * You can create and customize particles via the `Brenta::ParticleEmitter` class. All
 * the computation is done in the GPU so the engine can handle lots and lots
 * of particles. Here's a quick look on the API:
 * 
 * ```cpp
 * ParticleEmitter emitter = ParticleEmitter::Builder()
 *         .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
 *        .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
 *        .set_starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
 *        .set_starting_timeToLive(0.5f)
 *        .set_num_particles(1000)
 *        .set_spawn_rate(0.01f)
 *        .set_scale(1.0f)
 *        .set_atlas_path(std::filesystem::absolute(
 *            "assets/textures/particle_atlas.png"
 *        ).string())
 *        .set_atlas_width(8)
 *        .set_atlas_height(8)
 *        .set_atlas_index(45)
 *        .build();
 * 
 * // Inside the game loop:
 * emitter.updateParticles(Time::GetDeltaTime());
 * emitter.renderParticles();
 * ```
 *
 * \section logger Logger
 *
 * The logger is a simple logging system that allows you to log messages with different
 * levels. The levels are:
 * - **DEBUG**
 * - **INFO**
 * - **WARNING**
 * - **ERROR**
 * - **OUTPUT**
 * - **DISABLED**
 *
 * The logger will only log messages with a level equal or higher than the one you set.
 * You can also set a log file where the messages will be written, the default is `./logs/log.txt`.
 *
 * This is an example on how to log a message:
 * ```cpp
 * Brenta::Logger::Log(Brenta::Types::LogLevel::DEBUG, "This is a debug message: ", "hello");
 * DEBUG("This is a debug message with macro: ", "hello");
 * ```
 *
 * \section text Text
 *
 * The `Brenta::Text` subsystem allows you to render text on the screen. You can set the font and font size of your text, and render it in the main loop like this:
 * ```cpp
 * Text::RenderText("Hello OpenGL!", x, y, scale, glm::vec3(r, g, b));
 * ```
 *
 * \section more More
 *
 * This is just a small part of the engine, you can find more features in the
 * documentation of the classes. If you want to contribute to the engine, check out \ref Contributing.
 *
 * \page Contributing
 * First off, thank you for considering contributing to this project!
 * Even if this is not an advanced graphics engine, It provides
 * many opportunities for you to practice and improve your programmin skills.
 *
 * Building
 * an engine will test your **math skills** (linear algebra and calculus), 
 * **physics** knowledge through physics simulations, **algorithms and data
 * structures** (expecially for the ECS) and your knowledge of **modern
 * programming paradigms**. Overall, this is engiene is an easy place to improve
 * your skills.
 * 
 * \section how How to contribute
 *
 * You can look at the current <a href="https://github.com/San7o/Brenta-Engine/issues">issues</a> to get an idea of where the engine is going and where It
 * needs help. As the engine grows, help from contributors will be essential
 * to keep the project alive.
 *
 * \subsection bugs Reporting Bugs
 *
 * If you find a bug, please open an issue on the <a href="https://github.com/San7o/Brenta-Engine/issues">issues</a> page. Include as much detail as possible:
 * - A clear descriptive title
 * - Steps to reproduce the issue
 * - Expected behavior
 * - Version of the engine
 * - Any relevant screenshots or logs
 *
 * \subsection features Adding Features
 *
 * Suggersions for new features or improvements are always welcome.
 * To suggest a feature:
 * - Open an issue on the github repository
 * - Clearly describe the feature
 * - Explain why it would be beneficial to the engine
 *
 * \subsection code Contributing Code
 *
 * - fork the repository
 * - create a new branch
 * - make your changes
 * - push the branch to your fork
 * - open a pull request against the `dev` branch of the main repository
 *   with a clear description of the changes
 * 
 */
 // cmake-format: on
