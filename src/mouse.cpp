// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/mouse.hpp>

using namespace brenta;

// Getters

bool Mouse::get_first() const
{
  return this->first;
}

float Mouse::get_x() const
{
  return this->x;
}

float Mouse::get_y() const
{
  return this->y;
}

float Mouse::get_sensitivity() const
{
  return this->sensitivity;
}

// Setters

void Mouse::set_first(bool first)
{
  this->first = first;
}

void Mouse::set_x(float x)
{
  this->x = x;
}

void Mouse::set_y(float y)
{
  this->y = y;
}

void Mouse::set_sensitivity(float sensitivity)
{
  this->sensitivity = sensitivity;
}
