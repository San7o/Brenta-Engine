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

using namespace Brenta;

std::unordered_map<Types::ShaderName, unsigned int> Shader::shaders;

unsigned int Shader::GetId (Types::ShaderName shader_name)
{
    if (Shader::shaders.find (shader_name) == Shader::shaders.end ())
    {
        return 0;
    }
    return Shader::shaders.at (shader_name);
}

/* Use/activate the shader */
void Shader::Use (Types::ShaderName shader_name)
{
    glUseProgram (Shader::GetId (shader_name));
    GLenum err;
    if ((err = glGetError ()) != GL_NO_ERROR)
    {
        ERROR ("Error using shader: ", shader_name);
        std::cout << err << std::endl;
    }
}

/* Utility uniform functions */
void Shader::SetBool (Types::ShaderName shader_name, const std::string &name,
                      bool value)
{
    glUniform1i (
        glGetUniformLocation (Shader::GetId (shader_name), name.c_str ()),
        (int) value);
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting bool value for shader: ", shader_name,
               " and name: ", name);
    }
}

void Shader::SetInt (Types::ShaderName shader_name, const std::string &name,
                     int value)
{
    glUniform1i (
        glGetUniformLocation (Shader::GetId (shader_name), name.c_str ()),
        value);
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting int value for shader: ", shader_name,
               " and name: ", name);
    }
}

void Shader::SetFloat (Types::ShaderName shader_name, const std::string &name,
                       float value)
{
    glUniform1f (
        glGetUniformLocation (Shader::GetId (shader_name), name.c_str ()),
        value);
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting float value for shader: ", shader_name,
               " and name: ", name);
    }
}
void Shader::SetMat4 (Types::ShaderName shader_name, const GLchar *name,
                      glm::mat4 value)
{
    unsigned int matLoc =
        glGetUniformLocation (Shader::GetId (shader_name), name);
    glUniformMatrix4fv (matLoc, 1, GL_FALSE, glm::value_ptr (value));
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting mat4 value for shader: ", shader_name,
               " and name: ", name);
    }
}

void Shader::SetVec3 (Types::ShaderName shader_name, const GLchar *name,
                      float x, float y, float z)
{
    unsigned int vecLoc =
        glGetUniformLocation (Shader::GetId (shader_name), name);
    glUniform3f (vecLoc, x, y, z);
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting vec3 value for shader: ", shader_name,
               " and name: ", name);
    }
}

void Shader::SetVec3 (Types::ShaderName shader_name, const GLchar *name,
                      glm::vec3 value)
{
    unsigned int vecLoc =
        glGetUniformLocation (Shader::GetId (shader_name), name);
    glUniform3f (vecLoc, value.x, value.y, value.z);
    if (glGetError () != GL_NO_ERROR)
    {
        ERROR ("Error setting vec3 value for shader: ", shader_name,
               " and name: ", name);
    }
}

/* utility function for checking shader
 * compilation/linking errors. */
void Shader::CheckCompileErrors (unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog (shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                      << "\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv (shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog (shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                      << "\n"
                      << infoLog << std::endl;
        }
    }
}
