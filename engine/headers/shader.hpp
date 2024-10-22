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

#include "engine_logger.hpp"

#include <algorithm>
#include <fstream>
#include <glad/glad.h> /* OpenGL driver */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace types
{

typedef std::string shader_name_t;

} // namespace types

/**
 * @brief Shader class
 *
 * This class is used to create and manage shaders. The shaders
 * are created using the New method, which takes the name of the
 * shader, the type of the shader, and the path to the file that
 * contains the shader code. Multile shaders can be compiled and
 * linked together by providing any number of types and paths.
 * The shader can be used with the Use method, and the uniforms
 * can be set using the SetBool, SetInt, SetFloat, SetMat4, SetVec3
 * methods.
 */
class shader
{
  public:
    /**
     * @brief Map of shaders
     *
     * This map is used to store the shaders that are created
     * during the execution of the program. The key is the name
     * of the shader and the value is the ID of the shader.
     */
    static std::unordered_map<types::shader_name_t, unsigned int> shaders;

    /**
     * @brief Create a new shader
     *
     * This method is used to create a new shader with the given
     * name, type, and path. The path is the path to the file that
     * contains the shader code. The type is the type of the shader
     * (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER).
     *
     * @param shader_name Name of the shader
     * @param type Type of the shader
     * @param path Path to the file that contains the shader code
     *
     * You can provide any number of types and paths, those will be
     * all compiled and linked in the same program.
     */
    template <typename... Args>
    static void create(std::string shader_name, GLenum type, std::string path,
                       Args... args)
    {
        std::vector<unsigned int> compiled_shaders = {};
        compile_shaders(compiled_shaders, type, path, args...);

        /* shader Program */
        unsigned int ID = glCreateProgram();
        std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                      [&ID](auto shader) { glAttachShader(ID, shader); });

        glLinkProgram(ID);
        shader::check_compile_errors(ID, "PROGRAM");

        shader::shaders.insert({shader_name, ID});
        std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                      [](auto shader) { glDeleteShader(shader); });
    }

    /**
     * @brief Create a new shader with feedback varyings
     *
     * @param feedback_varyings Array of feedback varyings
     * @param num_varyings Number of feedback varyings
     * @param shader_name Name of the shader
     * @param type Type of the shader
     * @param path Path to the file that contains the shader code
     *
     * Same as the New method, but adds feedback varyings to the shader,
     * so that the output of the shader can be saved in a buffer object.
     *
     * Example feedback_varyings:
     * const GLchar* feedback_varyings[] = {"outValue"};
     */
    template <typename... Args>
    static void create(const GLchar **feedback_varyings, int num_varyings,
                       std::string shader_name, GLenum type, std::string path,
                       Args... args)
    {
        std::vector<unsigned int> compiled_shaders = {};
        compile_shaders(compiled_shaders, type, path, args...);

        /* shader Program */
        unsigned int ID = glCreateProgram();
        std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                      [&ID](auto shader) { glAttachShader(ID, shader); });

        if (feedback_varyings != nullptr)
        {
            glTransformFeedbackVaryings(ID, num_varyings, feedback_varyings,
                                        GL_INTERLEAVED_ATTRIBS);
        }

        glLinkProgram(ID);
        shader::check_compile_errors(ID, "PROGRAM");

        shader::shaders.insert({shader_name, ID});
        std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                      [](auto shader) { glDeleteShader(shader); });
    }

    static void compile_shaders(std::vector<unsigned int> &compiled)
    {
        return;
    }

    template <typename... Args>
    static void compile_shaders(std::vector<unsigned int> &compiled,
                                GLenum type, std::string path, Args... args)
    {
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        }
        catch (std::ifstream::failure &e)
        {
            ERROR("Error reading shader file: {}", path);
            return;
        }

        const char *shader_code = code.c_str();
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &shader_code, NULL);
        glCompileShader(shader);
        shader::check_compile_errors(shader, "SHADER");

        compiled.push_back(shader);
        compile_shaders(compiled, args...);
    }

    /**
     * @brief Get the ID of a shader
     *
     * @param shader_name Name of the shader
     * @return ID of the shader
     */
    static unsigned int get_id(types::shader_name_t shader_name);

    /**
     * @brief Use the shader
     *
     * @param shader_name Name of the shader
     * You need to call this method before rendering anything
     * with the shader.
     */
    static void use(types::shader_name_t shader_name);

    /* Utility uniform functions */

    /**
     * @brief Set a boolean in the shader
     *
     * @param shader_name Name of the shader
     * @param name Name of the uniform boolean
     * @param value Value of the boolean
     */
    static void set_bool(types::shader_name_t shader_name,
                         const std::string &name, bool value);
    /**
     * @brief Set an integer in the shader
     *
     * @param shader_name Name of the shader
     * @param name Name of the uniform integer
     * @param value Value of the integer
     */
    static void set_int(types::shader_name_t shader_name,
                        const std::string &name, int value);
    /**
     * @brief Set a float in the shader
     *
     * @param shader_name Name of the shader
     * @param name Name of the uniform float
     * @param value Value of the float
     */
    static void set_float(types::shader_name_t shader_name,
                          const std::string &name, float value);
    /**
     * @brief Set a 4x4 matrix in the shader
     *
     * @param shader_name Name of the shader
     * @param name Name of the uniform matrix
     * @param value Value of the matrix
     */
    static void set_mat4(types::shader_name_t shader_name, const GLchar *name,
                         glm::mat4 value);
    /**
     * @brief Set a 3D vector in the shader
     *
     * @param shader_name Name of uniform the shader
     * @param name Name of the vector
     * @param x X value of the vector
     * @param y Y value of the vector
     * @param z Z value of the vector
     */
    static void set_vec3(types::shader_name_t shader_name, const GLchar *name,
                         float x, float y, float z);
    /**
     * @brief Set a 3D vector in the shader
     *
     * @param shader_name Name of the shader
     * @param name Name of the uniform vector
     * @param value Value of the vector
     */
    static void set_vec3(types::shader_name_t shader_name, const GLchar *name,
                         glm::vec3 value);

  private:
    static void check_compile_errors(unsigned int shader, std::string type);
};

} // namespace brenta
