/*
 *
 *     Created on: 2024-07-03
 *     Author: Giovanni Santini 
 *     License: MIT
 *
 */

#include "shader.h"

using namespace ECS;

std::unordered_map<Types::ShaderName, unsigned int> Shader::shaders;

unsigned int Shader::GetId(Types::ShaderName shader_name)
{
    if (Shader::shaders.find(shader_name) == Shader::shaders.end())
    {
        return 0;
    }
    return Shader::shaders.at(shader_name);
}

/* Use/activate the shader */
void Shader::Use(Types::ShaderName shader_name)
{
    glUseProgram(Shader::GetId(shader_name));
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error using shader: " + shader_name);
        std::cout << err << std::endl;
    }
}

/* Utility uniform functions */
void Shader::SetBool(Types::ShaderName shader_name, const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(Shader::GetId(shader_name),
                            name.c_str()), (int)value);
    if (glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting bool value for shader: " + shader_name + " and name: " + name);
    }
}

void Shader::SetInt(Types::ShaderName shader_name, const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(Shader::GetId(shader_name), name.c_str()), value); 
    if (glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting int value for shader: " + shader_name + " and name: " + name);
    }
}

void Shader::SetFloat(Types::ShaderName shader_name, const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(Shader::GetId(shader_name), name.c_str()), value);
    if (glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting float value for shader: " + shader_name + " and name: " + name);
    }
}
void Shader::SetMat4(Types::ShaderName shader_name, const GLchar* name, glm::mat4 value)
{
    unsigned int matLoc = glGetUniformLocation(Shader::GetId(shader_name), name);
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
    if(glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting mat4 value for shader: " + shader_name + " and name: " + name);
    }
}

void Shader::SetVec3(Types::ShaderName shader_name, const GLchar* name, float x, float y, float z)
{
    unsigned int vecLoc = glGetUniformLocation(Shader::GetId(shader_name), name);
    glUniform3f(vecLoc, x, y, z);
    if (glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting vec3 value for shader: " + shader_name + " and name: " + name);
    }
}

void Shader::SetVec3(Types::ShaderName shader_name, const GLchar* name, glm::vec3 value)
{
    unsigned int vecLoc = glGetUniformLocation(Shader::GetId(shader_name), name);
    glUniform3f(vecLoc, value.x, value.y, value.z);
    if (glGetError() != GL_NO_ERROR)
    {
        Logger::Log(Types::LogLevel::ERROR, "Error setting vec3 value for shader: " + shader_name + " and name: " + name);
    }
}

/* utility function for checking shader
 * compilation/linking errors. */
void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
                      << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                      << type << "\n" << infoLog << std::endl;
        }
    }
}
