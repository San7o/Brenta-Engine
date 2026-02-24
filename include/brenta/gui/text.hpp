// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/gui/font.hpp>
#include <brenta/renderer/color.hpp>

#include <map>
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
                     std::shared_ptr<Font> font);

};
  
} // namespace brenta
