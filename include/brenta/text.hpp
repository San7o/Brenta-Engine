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

  Text()  = delete;
  ~Text() = delete;
  
  static void render(const std::string &text,
                     float x, float y,
                     float scale,
                     Color color,
                     tenno::shared_ptr<Font> font);

};
  
} // namespace brenta
