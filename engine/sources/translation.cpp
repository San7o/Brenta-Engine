#include "translation.h"

using namespace ECS::Types;

Translation::Translation() {
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
    this->model = glm::mat4(1.0f);
}

void Translation::setView(glm::mat4 view) {
    this->view = view;
}

void Translation::setProjection(float fov, float near, float far) {
    this->projection = glm::perspective(glm::radians(fov), 
        (float) ECS::Screen::GetWidth() / (float) ECS::Screen::GetHeight(),
        near, far);
}

void Translation::setProjection(glm::mat4 projection) {
    this->projection = projection;
}

void Translation::setModel(glm::mat4 new_model) {
    this->model = new_model;
}

void Translation::translate(glm::vec3 translation) {
    this->model = glm::translate(this->model, translation);
}

void Translation::rotate(glm::vec3 rotation) {
    this->model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Translation::scale(float scale) {
    this->model = glm::scale(this->model, glm::vec3(scale));
}

void Translation::setShader(Types::ShaderName shader_name) {

    Shader::Use(shader_name);

    Shader::SetMat4(shader_name, "view", this->view);
    Shader::SetMat4(shader_name, "projection", this->projection);
    Shader::SetMat4(shader_name, "model", this->model);

}
