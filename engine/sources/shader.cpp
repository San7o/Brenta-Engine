/*
 *
 *     Created on: 2024-07-03
 *     Author: Giovanni Santini 
 *     License: MIT
 *
 */

#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace ECS;

/* Constructor reads and builds the shader */
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
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
    checkCompileErrors(vertex, "VERTEX");

    /* fragment Shader */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    /* shader Program */
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    /* delete the shaders as they're linked into 
     * our program now and no longer necessary */
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

/* Use/activate the shader */
void Shader::use()
{
    glUseProgram(ID);
}

/* Utility uniform functions */
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const GLchar* name, glm::mat4 value) const
{
    unsigned int matLoc = glGetUniformLocation(this->ID, name);
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const GLchar* name, float x, float y, float z) const
{
    unsigned int vecLoc = glGetUniformLocation(this->ID, name);
    glUniform3f(vecLoc, x, y, z);
}

/* utility function for checking shader
 * compilation/linking errors. */
void Shader::checkCompileErrors(unsigned int shader, std::string type)
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
