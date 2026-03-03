// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/font.hpp>
#include <brenta/color.hpp>

#include <string>

namespace brenta
{

class Text
{
public:

  std::string             text;
  float                   x;
  float                   y;
  float                   scale;
  Color                   color;
  tenno::shared_ptr<Font> font;
  
  Text()  = default;
  Text(const std::string &text,
       float x,
       float y,
       float scale,
       Color color,
       tenno::shared_ptr<Font> font)
    : text(text), x(x), y(y), scale(scale), color(color), font(font)
  {}
  ~Text() = default;

  //
  // Static API
  //
  
  static void render(const std::string &text,
                     float x,
                     float y,
                     float scale,
                     Color color,
                     Font& font);
  static void render(const Text& text);

};
  
} // namespace brenta
