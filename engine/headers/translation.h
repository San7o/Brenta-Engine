#pragma once

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ECS {

namespace Types {

class Translation {
public:
    glm::mat4 view;       /* Camera */
    glm::mat4 projection; /* Based on perspective */
    glm::mat4 model;      /* Object position */

    Translation();

    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);
    void setProjection(float fov, float near, float far);
    void setModel(glm::mat4 model);
    void translate(glm::vec3 translation);
    void rotate(glm::vec3 rotation);
    void scale(float scale);
    void setShader(Types::ShaderName shader_name);
};

} // namespace Types

} // namespace ECS

#endif
