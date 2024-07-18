/*
 *
 *     Created on: 2024-07-03
 *     Author: Giovanni Santini 
 *     License: MIT
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef __glad_h_
#include <glad/glad.h>       /* OpenGL driver */
#endif

#include <unordered_map>

namespace ECS {

namespace Types {

typedef std::string ShaderName;

} // namespace Types

class Shader {

public:
    static std::unordered_map<Types::ShaderName, unsigned int> shaders;

    /* Constructor reads and builds the shader */
    static void NewShader(Types::ShaderName shader_name,
                          std::string const& vertexPath,
                          std::string const& fragmentPath);
    static unsigned int GetId(Types::ShaderName shader_name);

    /* Use/activate the shader */
    static void Use(Types::ShaderName shader_name);

    /* Utility uniform functions */
    static void SetBool(Types::ShaderName shader_name,
                        const std::string &name, bool value);
    static void SetInt(Types::ShaderName shader_name,
                        const std::string &name, int value);
    static void SetFloat(Types::ShaderName shader_name,
                        const std::string &name, float value);
    static void SetMat4(Types::ShaderName shader_name,
                        const GLchar* name, glm::mat4 value);
    static void SetVec3(Types::ShaderName shader_name,
                        const GLchar* name, float x, float y, float z);
private:
    static void CheckCompileErrors(unsigned int shader, std::string type);
};

} // namespace ECS

#endif
