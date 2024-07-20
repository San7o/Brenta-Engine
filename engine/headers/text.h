#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "engine.h"

namespace ECS {

namespace Types {

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

} // namespace Types

class Text {
public:
    static std::map<char, Types::Character> characters;

    Text() = delete;
    /* Initialize only after OpenGL context is created */
    static void Init();

    static void Load(std::string font_name, unsigned int fontSize);
    static void RenderText(std::string text, float x,
                    float y, float scale, glm::vec3 color);
private:
    static Types::ShaderName textShader;
    static Types::VAO        textVao;
    static Types::Buffer     textVbo;
};

} // namespace ECS

#endif
