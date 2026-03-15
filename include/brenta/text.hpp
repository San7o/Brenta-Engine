// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/color.hpp>

#include <tenno/memory.hpp>

#include <string>

namespace brenta
{

class Font;
  
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
  
  void render(int width, int height) const;

};
  
} // namespace brenta
