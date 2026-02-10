// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/mouse.hpp>

using namespace brenta;

// Getters

bool mouse::get_first() const
{
  return this->first;
}

float mouse::get_x() const
{
  return this->x;
}

float mouse::get_y() const
{
  return this->y;
}

float mouse::get_sensitivity() const
{
  return this->sensitivity;
}

// Setters

void mouse::set_first(bool first)
{
  this->first = first;
}

void mouse::set_x(float x)
{
  this->x = x;
}

void mouse::set_y(float y)
{
  this->y = y;
}

void mouse::set_sensitivity(float sensitivity)
{
  this->sensitivity = sensitivity;
}
