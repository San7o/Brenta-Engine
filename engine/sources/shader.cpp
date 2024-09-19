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

#include "shader.hpp"

using namespace brenta;

std::unordered_map<types::shader_name_t, unsigned int> shader::shaders;

unsigned int shader::get_id(types::shader_name_t shader_name)
{
    if (shader::shaders.find(shader_name) == shader::shaders.end())
    {
        return 0;
    }
    return shader::shaders.at(shader_name);
}

/* Use/activate the shader */
void shader::use(types::shader_name_t shader_name)
{
    glUseProgram(shader::get_id(shader_name));
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        ERROR("Error using shader: {}", shader_name);
        std::cout << err << std::endl;
    }
}

/* Utility uniform functions */
void shader::set_bool(types::shader_name_t shader_name, const std::string &name,
                      bool value)
{
    glUniform1i(glGetUniformLocation(shader::get_id(shader_name), name.c_str()),
                (int) value);
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting bool value for shader: {} and name: {}",
              shader_name, name);
    }
}

void shader::set_int(types::shader_name_t shader_name, const std::string &name,
                     int value)
{
    glUniform1i(glGetUniformLocation(shader::get_id(shader_name), name.c_str()),
                value);
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting int value for shader: {} and name: {}",
              shader_name, name);
    }
}

void shader::set_float(types::shader_name_t shader_name,
                       const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(shader::get_id(shader_name), name.c_str()),
                value);
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting float value for shader: {} and name: {}",
              shader_name, name);
    }
}

void shader::set_mat4(types::shader_name_t shader_name, const GLchar *name,
                      glm::mat4 value)
{
    unsigned int matLoc =
        glGetUniformLocation(shader::get_id(shader_name), name);
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting mat4 value for shader: {} and name: {}",
              shader_name, name);
    }
}

void shader::set_vec3(types::shader_name_t shader_name, const GLchar *name,
                      float x, float y, float z)
{
    unsigned int vecLoc =
        glGetUniformLocation(shader::get_id(shader_name), name);
    glUniform3f(vecLoc, x, y, z);
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting vec3 value for shader: {} and name: {}",
              shader_name, name);
    }
}

void shader::set_vec3(types::shader_name_t shader_name, const GLchar *name,
                      glm::vec3 value)
{
    unsigned int vecLoc =
        glGetUniformLocation(shader::get_id(shader_name), name);
    glUniform3f(vecLoc, value.x, value.y, value.z);
    if (glGetError() != GL_NO_ERROR)
    {
        ERROR("Error setting vec3 value for shader: {} and name: {}",
              shader_name, name);
    }
}

/* utility function for checking shader
 * compilation/linking errors. */
void shader::check_compile_errors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                      << "\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                      << "\n"
                      << infoLog << std::endl;
        }
    }
}
