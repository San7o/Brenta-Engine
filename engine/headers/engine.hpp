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
#include "frame_buffer.hpp"
#include "gl_helper.hpp"
#include "gui.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "particles.hpp"
#include "screen.hpp"
#include "shader.hpp"
#include "text.hpp"
#include "texture.hpp"
#include "translation.hpp"
#include "vao.hpp"

namespace brenta
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
class engine
{
  public:
    bool uses_screen;
    bool uses_audio;
    bool uses_input;
    bool uses_logger;
    bool uses_text;
    int screen_width;
    int screen_height;
    bool screen_is_mouse_captured;
    const char *screen_title;
    bool screen_msaa;
    bool screen_vsync;
    oak::level log_level;
    std::string log_file;
    std::string text_font;
    int text_size;
    bool gl_blending;
    bool gl_cull_face;
    bool gl_multisample;
    bool gl_depth_test;

    engine(bool uses_screen, bool uses_audio, bool uses_input, bool uses_logger,
           bool uses_text, int screen_width, int screen_height,
           bool screen_is_mouse_captured, bool screen_msaa, bool screen_vsync,
           const char *screen_title, oak::level log_level,
           std::string log_file, std::string text_font, int text_size,
           bool gl_blending, bool gl_cull_face, bool gl_multisample,
           bool gl_depth_test);
    ~engine();

    class builder;
};

/**
 * @brief Engine builder
 *
 * This class is used to build the engine. You can set the parameters of the
 * engine using the methods of this class and then call the build method to
 * create the engine.
 */
class engine::builder
{
  public:
    bool uses_screen = false;
    bool uses_audio = false;
    bool uses_input = false;
    bool uses_logger = false;
    bool uses_text = false;
    int screen_width = 1280;
    int screen_height = 720;
    bool screen_is_mouse_captured = false;
    bool screen_msaa = false;
    bool screen_vsync = false;
    const char *screen_title = "";
    oak::level log_level = oak::level::info;
    std::string log_file = "";
    std::string text_font = "arial.ttf";
    int text_size = 48;
    bool gl_blending = true;
    bool gl_cull_face = true;
    bool gl_multisample = true;
    bool gl_depth_test = true;

    builder &use_screen(bool uses_screen);
    builder &use_audio(bool uses_audio);
    builder &use_input(bool uses_input);
    builder &use_logger(bool uses_logger);
    builder &use_text(bool uses_text);
    builder &set_screen_width(int screen_width);
    builder &set_screen_height(int screen_height);
    builder &set_screen_is_mouse_captured(bool screen_is_mouse_captured);
    builder &set_screen_title(const char *screen_title);
    builder &set_screen_msaa(bool screen_msaa);
    builder &set_screen_vsync(bool screen_vsync);
    builder &set_log_level(oak::level log_level);
    builder &set_log_file(std::string log_file);
    builder &set_text_font(std::string text_font);
    builder &set_text_size(int text_size);
    builder &set_gl_blending(bool gl_blending);
    builder &set_gl_cull_face(bool gl_cull_face);
    builder &set_gl_multisample(bool gl_multisample);
    builder &set_gl_depth_test(bool gl_depth_test);

    engine build();
};

} // namespace brenta

// Doxygen Documnetation:

// cmake-format: off
/*! \mainpage Home
 *
 * \image html brenta-engine-banner.png
 *
 * \section intro_sec Introduction
 *
 * Hi and welcome to the **Brenta Engine documentation**! Brenta Engine is a
 * simple 3D engine written in modern  C++/OpenGL using the Entity Component
 * System architecture. The engine was created by Giovanni Santini in the summer
 * of 2024, the name is inspired by the Brenta Dolimites in the Italian Alps.
 *
 * \image html adamello-brenta.jpg
 *
 * \section submodules The Submodules
 *
 * The Engine is composed of many different submodules, each one with its own
 * purpose. The submodules are mostly independent from each other so you can use
 * any of the submodules you are interested into. I said mostly because all the
 * submodules that work in a graphical environment need the opengl context from
 * the Screen.
 *
 * The main submodules are:
 * - **brenta::screen**: manages the window and the OpenGL context.
 * - **brenta::audio**: everything audio.
 * - **brenta::input**: manages the screen input using callbacks.
 * - **brenta::text**: text rendering.
 * - **brenta::ecs::world**: the Entity Component System submodule.
 * - **brenta::engine**: manages the setup of the engine.
 * - **[oak](https://github.com/San7o/oak)**: engine's logger
 * - **[valfuzz](https://github.com/San7o/valFuzz)**: the engine's testing framework
 *
 * In addition, the engine has multiple classes to provide more functionalities.
 * Those are mostly wrappers around opengl primitives or handy classes providing
 * utilities. Classes with many variables also provide a Builder to nicely
 * iniitalize them.
 *
 * The classes are:
 * - **brenta::gl**: provides some useful OpenGL functions.
 * - **brenta::mesh**: a 3D openGL mesh.
 * - **brenta::model**: a 3D openGL model.
 * - **brenta::particle_emitter**: create and customize particles.
 * - **brenta::shader**: manages the shaders.
 * - **brenta::texture**: manages the textures.
 * - **brenta::types::translation**: manages the translations.
 * - **brenta::types::vao**: wrapper around the Vertex Array Objects.
 * - **brenta::types::buffer**: wrapper around the Buffers.
 * - **brenta::types::framebuffer**: framebuffer wrapper.
 * - **brenta::time**: manages the time.
 * - **brenta::camera**: manages the camera.
 * - **brenta::gui**: provides helper functions to ImGUI.
 *
 * \section graphics Graphics
 *
 * The engine uses OpenGL for the graphics and provides classes to interact with
 * opengl.
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
 * Although the engine currently implements only basic graphics features, it
 * provides all the building blocks to create more complex graphics. Effort will
 * be put into implementing various graphics techniques, if you want to
 * contribute check out the \ref Contributing page. <a
 * href="https://github.com/San7o/Brenta-Engine/issues/10">This</a> issue tracks
 * future features to implement.
 *
 * In the next section we are going to see how to build the engine.
 *
 * \section license License
 *
 * The engine is released under the MIT license. You can find the license in the
 * repository.
 *
 * \page Building
 *
 * The engine is written entirely in C++ and uses CMake as the build system. In
 * this section we will guide you through the process of building the engine.
 *
 * \section supported_platforms Supported Platforms
 *
 * Currently, the only officially supported platform is **Linux**. The developer
 * is aiming to release the engine on Windows and MacOS in the future. If you
 * want to contribute to this issue, feel free to do so and check out the \ref
 * Contributing page. This guide will go through the process on a linux system.
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
 * You may or may not have the libraries available in your packet manger, if not
 * you will need to build them from source. This guide will show you an example
 * of building libassimp.
 *
 * \subsection nix Nix
 *
 * If you are using Nix, a development shell is provided in the repository. You
 * can enter the shell by running the following command:
 * ```
 * nix develop
 * ```
 * You should now have all the dependencies installed and ready to build the
 * engine.
 *
 * \subsection libassimo Building libassimp
 *
 * You need <a href="https://github.com/assimp/assimp">libassimp</a> to load 3D
 * models. To build it from source, first clone the repository:
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
 * The library will be generated in `lib/`. If you don't install the library
 * system-wide (ASSIMP_INSTALL=off), you need to specify the path in an
 * environment variable to run the application, like so:
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
 * You can build the documentation with `doxygen` (you need to have doxygen
 * installed in your system):
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
 * The engine uses It's own testing framework, [valFuzz](https://github.com/San7o/valFuzz).
 * To build tests, run:
 * ```bash
 * cmake -Bbuild -DBRENTA_BUILD_TESTS=ON
 * cmake --build build -j 4
 * ./buid/tests --no-multithread
 * ```
 *
 * \section examples Examples
 *
 * There is an `examples` directory, you can run an exmple with the following
 * command:
 * ```bash
 * cmake -Bbuild -DBRENTA_BUILD_EXAMPLES=ON
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
 * - **utils**: contains utility classes like the Logger, those classes are
 * ussed both by the engine and the ECS
 * - **tests**: contains the tests for the engine
 * - **examples**: contains some examples on features of the engine, to build
 * them follow the instructions in the \ref Building page
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
 * The engine uses the `brenta` namespace. There are also some subnamespaces:
 * - **brenta::types**: contains the types used in the engine
 * - **brenta::ecs**: contains the types used in the ECS
 *
 * \page ECS
 * Brenta Engine features an Entity Component System architecture. The ECS is a
 * design pattern that allows you to structure your code in a way that is more
 * modular and scalable.
 *
 * The ECS is composed of three main parts:
 * - **Entity**: an entity is an object in the game, it is just an ID
 * - **Component**: a component is a piece of data that is attached to an entity
 * - **System**: a system is a piece of code that operates on entities with
 * specific components. They get autocamically executed at each game tick.
 * - **Resources**: resources are shared global data that can be accessed by any
 * system
 *
 * With those building blocks, you can create any game or application. The ECS
 * is optional, you can use the engine without it.
 *
 * \subsection header Header
 * Everything you need to use the ECS is in the `ecs` directory. To use the ECS,
 * include the `ecs.hpp` header:
 * ```cpp
 * #include "ecs.hpp"
 * using namespace brenta::ecs;
 * ```
 *
 * \section world The World
 *
 * Everything that exists in the ECS is contained in the `World` class. The
 * World is a singleton accessible from anywhere in the code and It will be your
 * entry point to the ECS. You can create entities, assign components to them,
 * and query entities with specific components. Let's take a look on how to use
 * the World API.
 *
 * \section entity Creating new Entities
 * Creating an entity is very straightforward, you can ask the world to create
 * one for you:
 * ```cpp
 * entity_t entity = world::new_entity();
 * ```
 * This will return a new entity with a unique ID or -1 if the entity could not
 * be created.
 *
 * \section component Assigning Components to Entities
 *
 * To assign a component to an entity, first you need to create a component.
 * Components are just structs with data that extend the `Component` class. Note
 * that you need to implement a default empty constructor for your component.
 *
 * For example:
 * ```cpp
 * struct position_component : public component {
 *     float x, y, z;
 *     position_component(); // Required
 *     position_component(float x, float y, float z); // Optional
 * }
 * ```
 *
 * You can find many more examples in `examples/components.cpp`.
 *
 * Once you have created a component, you can assign it to an entity:
 * ```cpp
 * position_component position(0.0f, 0.0f, 0.0f);
 * world::add_component<position_component>(entity, position);
 * ```
 *
 * \section system Creating Systems
 *
 * Systems are classes that extend the `system` class. Systems can query
 * entities with specific components and operate on them. They have a run method
 * that will be called at each tick. Systems can define which components to
 * query by specifying them in the template arguments (see the example below),
 * they will receive a vector of entities that match the query.
 *
 * **Note** that if your System does not need to query any component, you need
 * to pass `None` as the template argument.
 *
 * You can create a system like so:
 *
 * ```cpp
 *
 * struct renderer_system : system<model_component, transform_component> {
 *   // you need to implement the run method
 *   void run(std::vector<entity_t> matches) const override {
 *     if (matches.empty()) return;
 *
 *     for (auto match : matches) {
 *         // get the components associated to an entity
 *         auto model_component =
 *                world::entity_to_component<model_component>(match);
 *         auto my_model = model_component->mod;
 *
 *         // ...
 *
 *         my_model.draw(default_shader);
 *     }
 *   }
 * };
 * ```
 * Note how we used `world::entity_to_component` to get the components
 * associated with an entity. This is a common pattern you will use in your
 * systems.
 *
 * After you defined your system, you need to register it. This can be done
 * throught a macro:
 * ```cpp
 * REGISTER_SYSTEMS(renderer_system);
 * ```
 * **Note**: You can register as many systems as you want but passing the
 * systems as a comma separated list to the macro, but you have to use
 * `REGISTER_SYSTEM` once and only once in your code, this is because systems
 * are registered in a global type.
 *
 * \section resources Resources
 *
 * Resources are data, like components, but they are not associated with
 * entities. Things like the day in the game, the number of enemies, etc should
 * be resources.
 *
 * You can create a resource like so:
 * ```cpp
 * struct day_resource : public resource {
 *    int day;
 *    day_resource() : day(0) {}
 *    day_resource(int day) : day(day) {}
 * };
 * ```
 *
 * You can register a resource like so:
 * ```cpp
 * world::add_resource<day_resource>(day_resource(1337));
 * ```
 *
 *
 * \page Engine
 * In this section we will see how to use various features of the engine.
 *
 * \section engine Engine Class
 * To initialize any subsystem, you can use the Engine class. This is not
 * mandatory, you can initialize and destroy the subsystems manually, but the
 * Engine class provides a nice way to do it.
 *
 * To create an engine, you can use the Builder class:
 * ```cpp
 * brenta::engine eng = brenta::engine::builder()
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
 * You can read every option in the `brenta::engine::builder` class.
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
 *     auto wireframe = world::get_resource("WireframeResource");
 *     if (wireframe == nullptr) return;
 *
 *     gl::set_poligon_mode(!wireframe->enabled);
 *     wireframe->enabled = !wireframe->enabled;
 * };
 *
 * input::add_keyboard_callback(GLFW_KEY_F, toggle_wireframe_callback);
 * ```
 * In this example we register a keyboard callback that toggles the wireframe
 * mode when the `F` key is pressed. You can use
 * `Brenta::Input::AddMousePosCallback` to register a mouse callback, this ill
 * be called with the x and y position of the mouse.
 *
 * You can also remove the callbacks with
 * `brenta::input::remove_keyboard_callback` and
 * `brenta::input::remove_mouse_pos_callback`.
 *
 * \section audio Audio
 *
 * The audio subsystem is very simple: there are audio streams and audio files,
 * you can play an audio file on a stream (not more) and stop it, so you need
 * to have multiple streams if you want to play multiple audio files at the same
 * time.
 *
 * You can load an audio file like so:
 * ```cpp
 * audio::load_audio("guitar", "assets/audio/guitar.wav");
 * ```
 * We are identifying this audio file with the name `guitar`.
 *
 * You can create a stream with the name "music" like so:
 * ```cpp
 * audio::create_stream("music");
 * ```
 *
 * And finally play the "guitar" audio like so:
 * ```cpp
 * sudio::play_audio("guitar", "music");
 * ```
 * The subsystem will provide you a default stream named "default" if you don't
 * want to create a stream.
 *
 * You can Pause and Resume streams with `brenta::audio::pause_stream` and
 * `brenta::audio::resume_stream`, set the volume and stop it. You can find
 * the API in `Brenta::Audio`.
 *
 * \section particles Particles
 *
 * You can create and customize particles via the `brenta::particle_emitter`
 * class. All the computation is done in the GPU so the engine can handle lots
 * and lots of particles. Here's a quick look on the API:
 *
 * ```cpp
 * particle_emitter emitter = particle_emitter::builder()
 *        .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
 *        .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
 *        .set_starting_spread(glm::vec3(10.0f, 10.0f, 10.0f))
 *        .set_starting_time_to_live(0.5f)
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
 * emitter.update_particles(time::get_delta_time());
 * emitter.render_particles();
 * ```
 *
 * \section logger Logger
 *
 * Check out [oak](https://github.com/San7o/oak)! The engine uses oak as the
 * logger, you can set the log level and the log file in the engine builder.
 * You can log messages like so:
 * ```cpp
 * oak::info("Hello, world!");
 * ```
 * Oak has many more advanced features, I suggest you check out the repository.
 *
 * \section text Text
 *
 * The `brenta::text` subsystem allows you to render text on the screen. You can
 * set the font and font size of your text, and render it in the main loop like
 * this:
 * ```cpp
 * text::render_text("Hello OpenGL!", x, y, scale, glm::vec3(r, g, b));
 * ```
 *
 * \section more More
 *
 * This is just a small part of the engine, you can find more features in the
 * documentation of the classes. If you want to contribute to the engine, check
 * out \ref Contributing.
 *
 * \page Contributing
 * First off, thank you for considering contributing to this project!
 * Even if this is not an advanced graphics engine, It provides
 * many opportunities for you to practice and improve your programming skills.
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
 * You can look at the current <a
 * href="https://github.com/San7o/Brenta-Engine/issues">issues</a> to get an
 * idea of where the engine is going and where It needs help. As the engine
 * grows, help from contributors will be essential to keep the project alive.
 *
 * \subsection bugs Reporting Bugs
 *
 * If you find a bug, please open an issue on the <a
 * href="https://github.com/San7o/Brenta-Engine/issues">issues</a> page. Include
 * as much detail as possible:
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
