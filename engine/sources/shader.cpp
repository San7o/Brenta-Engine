/*
 *
 *     Created on: 2024-07-03
 *     Author: Giovanni Santini 
 *     License: MIT
 *
 */

#include "shader.h"
#include "engine_logger.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace ECS;

std::unordered_map<Types::ShaderName, unsigned int> Shader::shaders;

/* Constructor reads and builds the shader */
void Shader::NewShader(Types::ShaderName shader_name,
                std::string const& vertexPath,
                std::string const& fragmentPath)
{
    if (Shader::shaders.find(shader_name) != Shader::shaders.end())
    {
        Logger::Log(Types::LogLevel::INFO, "Tried to create a shader that already exists with name: " + shader_name + ", the shader was not created");
        return;
    }

    /* 
     * 1. retrieve the vertex/fragment source
     *    code from filePath 
     */

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    /* Ensure ifstream objects can throw exceptions */
    vShaderFile.exceptions(std::ifstream::failbit |
                             std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit |
                             std::ifstream::badbit);

    try 
    {
        /* open files */
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        /* read file's buffer contents into streams */
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        /* close file handlers */
        vShaderFile.close();
        fShaderFile.close();

        /* convert stream into string */
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: "
                  << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    /* 
     * 2. compile shaders
     */

    unsigned int vertex, fragment;

    /* vertex shader */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    Shader::CheckCompileErrors(vertex, "VERTEX");

    /* fragment Shader */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    Shader::CheckCompileErrors(fragment, "FRAGMENT");

    /* shader Program */
    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    Shader::CheckCompileErrors(ID, "PROGRAM");

    /* Save the shader */
    Shader::shaders.insert({shader_name, ID});

    /* delete the shaders as they're linked into 
     * our program now and no longer necessary */
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

unsigned int Shader::GetId(Types::ShaderName shader_name)
{
    if (Shader::shaders.find(shader_name) == Shader::shaders.end())
    {
        return -1;
    }
    return Shader::shaders.at(shader_name);
}

/* Use/activate the shader */
void Shader::Use(Types::ShaderName shader_name)
{
    glUseProgram(Shader::GetId(shader_name));
}

/* Utility uniform functions */
void Shader::SetBool(Types::ShaderName shader_name, const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(Shader::GetId(shader_name),
                            name.c_str()), (int)value);
}

void Shader::SetInt(Types::ShaderName shader_name, const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(Shader::GetId(shader_name), name.c_str()), value); 
}

void Shader::SetFloat(Types::ShaderName shader_name, const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(Shader::GetId(shader_name), name.c_str()), value);
}
void Shader::SetMat4(Types::ShaderName shader_name, const GLchar* name, glm::mat4 value)
{
    unsigned int matLoc = glGetUniformLocation(Shader::GetId(shader_name), name);
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(Types::ShaderName shader_name, const GLchar* name, float x, float y, float z)
{
    unsigned int vecLoc = glGetUniformLocation(Shader::GetId(shader_name), name);
    glUniform3f(vecLoc, x, y, z);
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
