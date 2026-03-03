// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace brenta;
using namespace brenta::types;

TEST(new_translation, "Create a Translation")
{
  translation t = translation();
  ASSERT(t.view == glm::mat4(1.0f));
  ASSERT(t.projection == glm::mat4(1.0f));
  ASSERT(t.model == glm::mat4(1.0f));

  glm::mat4 view = glm::mat4(3.0f);
  glm::mat4 projection = glm::mat4(3.0f);
  glm::mat4 model = glm::mat4(3.0f);
  t = translation(view, projection, model);
  ASSERT(t.view == view);
  ASSERT(t.projection == projection);
  ASSERT(t.model == model);
}

TEST(view, "set view")
{
  translation t = translation();
  t.set_view(glm::mat4(3.0f));
  ASSERT(t.view == glm::mat4(3.0f));
}

TEST(model_test, "set model")
{
  translation t = translation();
  t.set_model(glm::mat4(3.0f));
  ASSERT(t.model == glm::mat4(3.0f));
}

TEST(translate, "translate")
{
  translation t = translation();
  t.translate(glm::vec3(1.0f, 2.0f, 3.0f));
  ASSERT(t.model
         == glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f)));
}

TEST(rotate, "rotate")
{
  translation t = translation();
  t.rotate(glm::vec3(1.0f, 2.0f, 3.0f));
  glm::mat4 model = glm::mat4(1.0);
  model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ASSERT(t.model == model);
}

TEST(scale, "scale")
{
  translation t = translation();
  t.scale(2.0f);
  ASSERT(t.model == glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)));
}
